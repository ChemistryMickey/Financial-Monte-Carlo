// main.cpp
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>        // optional
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include "implot.h"

#include <GLFW/glfw3.h>

#include "Percentiles.hpp"
#include "csv.hpp"
#include "indicators.hpp"

#include <vector>
#include <string>
#include <iostream>
#include <filesystem>
#include <algorithm>
#include <map>

static const std::map<const char*, ImVec4> color_map = {
    {"blue", {0.0f, 0.0f, 0.8f, 1.0f}},
    {"red", {0.8f, 0.0f, 0.0f, 1.0f}},
    {"green", {0.0f, 0.8f, 0.0f, 1.0f}},
    {"yellow", {0.8f, 0.8f, 0.0f, 1.0f}},
    {"orange", {1.0f, 0.5f, 0.0f, 1.0f}},
    {"white", {1.0f, 1.0f, 1.0f, 1.0f}},
    {"black", {0.0f, 0.0f, 0.0f, 1.0f}},
    {"grey", {0.5f, 0.5f, 0.5f, 0.5f}},
};
static const std::map<const char*, const char*> percentile_to_color_map = {
        {"Min", "black"},
        {"5th Percentile", "red"},
        {"25th Percentile", "orange"},
        {"50th Percentile (Median)", "green"},
        {"75th Percentile", "orange"},
        {"95th Percentile", "red"},
        {"Max", "black"},
};

indicators::ProgressBar make_simple_progress_bar(const char* postfix) {
    using namespace indicators;
    return ProgressBar{
        option::BarWidth{50},
        option::Start{"["},
        option::Fill{"="},
        option::Lead{">"},
        option::Remainder{" "},
        option::End{"]"},
        option::PostfixText{postfix},
        option::ForegroundColor{Color::green},
        option::ShowPercentage{true},
        option::FontStyles{std::vector<FontStyle>{FontStyle::bold}}
    };
}

int main(int argc, char** argv) {
    using namespace fmc;
    static_cast<void>(argc);

    // Do all the loading and math up front. This way it can be offloaded to a thread later.
    // Recursively load CSV files
    std::string rootDir = argv[1];
    std::vector<std::filesystem::directory_entry> entries;
    for (const auto& entry : std::filesystem::recursive_directory_iterator(rootDir)) {
        if (entry.path().extension() == ".csv") {
            entries.push_back(entry);
        }
    }

    size_t n_filesFound = entries.size();
    if (n_filesFound == 0) {
        std::cerr << "No CSV files found!\n";
        return 1;
    }

    std::vector<TimeSeriesFile> runs{};
    runs.reserve(n_filesFound);
    {
        size_t fileIdx = 0;
        auto bar = make_simple_progress_bar("Loading CSVs");
        for (const auto& f : entries) {
            runs.push_back(loadCsv(f.path().string()));

            fileIdx += 1;
            bar.set_progress((double) fileIdx / (double) n_filesFound * 100.0);
        }
    }
    if (runs.size() == 0) {
        throw std::runtime_error("No CSV files loaded!");
    }

    size_t numCols = runs[0].columns.size();
    std::vector<PercentileStats> columnStats(numCols);

    // Precompute stats
    {
        auto bar = make_simple_progress_bar("Computing Statistics");
        for (size_t c = 0; c < numCols; ++c) {
            std::vector<std::vector<double>> allRuns;
            for (auto& run : runs) {
                allRuns.push_back(run.columns[c]);
            }
            columnStats[c] = fmc::computeStats(allRuns);
            bar.set_progress((double) (c + 1) / (double) numCols * 100);
        }
    }

    // Make window
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(1280, 720, "IMPlot Monte Carlo Visualizer", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    // Options
    // Timeseries data
    // Why are these strings? Because of sorting/hashing
    std::map<std::string, bool> timeseries_checkboxes = {
        {"Raw Data", false},
        {"Single Run", false},
        {"Min", false},
        {"5th Percentile", true},
        {"25th Percentile", true},
        {"50th Percentile (Median)", true},
        {"75th Percentile", true},
        {"95th Percentile", true},
        {"Max", false},
        {"Log Y Axis", false}
    };
    int rawRunRange[2] = {0, 10};
    int singleRunInd = 0;

    // Dispersions
    // TODO

    // Render Loop
    size_t selectedColumn = 0;
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Timeseries plot controls
        ImGui::Begin("Controls");
        if (ImGui::BeginCombo("Y Axis", runs[0].columnNames[selectedColumn].c_str())) {
            for (size_t i = 0; i < numCols; ++i) {
                bool is_selected = (selectedColumn == i);
                if (ImGui::Selectable(runs[0].columnNames[i].c_str(), is_selected)) {
                    selectedColumn = i;
                }
                if (is_selected) {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }
        for (auto& [name, show] : timeseries_checkboxes) {
            ImGui::Checkbox(name.c_str(), &show);
        }
        ImGui::SeparatorText("Numerical Input Options");
        ImGui::InputInt2("Raw Data Run Range", rawRunRange);
        ImGui::InputInt("Single Run Index", &singleRunInd);
        ImGui::End();

        // Validate Inputs
        rawRunRange[0] = std::clamp(rawRunRange[0], 0, rawRunRange[1]);
        rawRunRange[1] = std::clamp(rawRunRange[1], rawRunRange[0], (int) n_filesFound - 1);
        singleRunInd = std::clamp(singleRunInd, 0, (int) n_filesFound - 1);

        // Timeseries plot
        ImGui::Begin("Timeseries Plots");
        if (ImPlot::BeginPlot("Time Series", ImVec2{-1, -1})) {

            auto& stats = columnStats[selectedColumn];
            std::map<const char*, double*> stats_name_map = {
                {"Min", stats.min.data()},
                {"5th Percentile", stats.p5.data()},
                {"25th Percentile", stats.p25.data()},
                {"50th Percentile (Median)", stats.p50.data()},
                {"75th Percentile", stats.p75.data()},
                {"95th Percentile", stats.p95.data()},
                {"Max", stats.max.data()},
            };

            const auto& colName = runs[0].columnNames[selectedColumn];

            ImPlot::SetupAxes("Time", colName.c_str());
            ImPlot::SetupAxisScale(ImAxis_Y1, timeseries_checkboxes["Log Y Axis"] ? ImPlotScale_Log10 : ImPlotScale_Linear);

            auto show_data = [&](bool show, ImVec4 color, const char* name, double* time, double* data, int size, float lineweight = 3.0f) {
                if (show) {
                    ImPlot::PushStyleVar(ImPlotStyleVar_LineWeight, lineweight);
                    ImPlot::PushStyleColor(ImPlotCol_Line, color);

                    ImPlot::PlotLine(
                        name,
                        time,
                        data,
                        size
                    );

                    ImPlot::PopStyleColor();
                    ImPlot::PopStyleVar();
                }

                };

            // Raw runs
            if (timeseries_checkboxes["Raw Data"]) {
                ImPlot::PushStyleVar(ImPlotStyleVar_LineWeight, 1.0f);
                ImPlot::PushStyleColor(ImPlotCol_Line, ImVec4(0.7f, 0.7f, 0.7f, 0.4f));

                for (size_t r = rawRunRange[0]; r <= rawRunRange[1]; ++r) {
                    ImPlot::PlotLine(
                        ("Run " + std::to_string(r)).c_str(),
                        runs[r].time.data(),
                        runs[r].columns[selectedColumn].data(),
                        (int) runs[r].time.size(),
                        ImPlotFlags_None
                    );
                }

                ImPlot::PopStyleColor();
                ImPlot::PopStyleVar();
            }

            for (auto& [name, data_ptr] : stats_name_map) {
                show_data(
                    timeseries_checkboxes.at(name),
                    color_map.at(percentile_to_color_map.at(name)),
                    name,
                    runs[0].time.data(),
                    data_ptr,
                    (int) runs[0].time.size()
                );
            }
            show_data(timeseries_checkboxes.at("Single Run"),
                color_map.at("white"),
                ("Run " + std::to_string(singleRunInd)).c_str(),
                runs[singleRunInd].time.data(),
                runs[singleRunInd].columns[selectedColumn].data(),
                (int) runs[singleRunInd].time.size()
            );

            ImPlot::EndPlot();
        }
        ImGui::End();

        // Render
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    ImPlot::DestroyContext();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
}