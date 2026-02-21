// main.cpp
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>        // optional
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include "implot.h"

#include <GLFW/glfw3.h>

#include "Percentiles.hpp"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iostream>
#include <map>


// ============================================================
// Data Structures
// ============================================================

struct TimeSeriesFile {
    std::vector<double> time;
    std::vector<std::string> columnNames;
    std::vector<std::vector<double>> columns;
};


// ============================================================
// CSV Loader
// ============================================================

TimeSeriesFile LoadCSV(const std::string& path) {
    TimeSeriesFile result;
    std::ifstream file(path);
    std::string line;

    if (!file.is_open()) return result;

    // Read header
    std::getline(file, line);
    std::stringstream header(line);
    std::vector<std::string> headers;
    {
        std::string cell;

        while (std::getline(header, cell, ',')) {
            headers.push_back(cell);
        }
    }

    // First column is time
    result.columnNames.assign(headers.begin() + 1, headers.end());

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string cell;
        std::vector<double> row;

        while (std::getline(ss, cell, ',')) {
            row.push_back(std::stod(cell));
        }

        if (result.columns.empty()) {
            result.columns.resize(row.size() - 1);
        }

        result.time.push_back(row[0]);
        for (size_t i = 1; i < row.size(); ++i) {
            result.columns[i - 1].push_back(row[i]);
        }
    }

    return result;
}

// ============================================================
// Main
// ============================================================

int main(int argc, char** argv) {
    static_cast<void>(argc);

    glfwInit();
    GLFWwindow* window = glfwCreateWindow(1280, 720, "IMPlot Monte Carlo Visualizer", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    std::vector<TimeSeriesFile> runs;

    // Recursively load CSV files
    std::string rootDir = argv[1];
    for (auto& p : std::filesystem::recursive_directory_iterator(rootDir)) {
        if (p.path().extension() == ".csv") {
            runs.push_back(LoadCSV(p.path().string()));
        }
    }

    if (runs.empty()) {
        std::cerr << "No CSV files found!\n";
        return 1;
    }

    size_t selectedColumn = 0;
    bool showRaw = true;
    bool showMedian = true;
    bool showP5 = true;
    bool showP95 = true;
    bool showP50 = true;
    bool logY = false;

    auto& time = runs[0].time;
    size_t numCols = runs[0].columns.size();

    std::vector<fmc::PercentileStats> columnStats(numCols);

    // Precompute stats
    for (size_t c = 0; c < numCols; ++c) {
        std::vector<std::vector<double>> allRuns;
        for (auto& run : runs)
            allRuns.push_back(run.columns[c]);
        columnStats[c] = fmc::ComputeStats(allRuns);
    }

    // ========================================================
    // Render Loop
    // ========================================================

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Controls");
        if (ImGui::BeginCombo("Y Axis", runs[0].columnNames[selectedColumn].c_str())) {
            for (size_t i = 0; i < numCols; ++i) {
                bool is_selected = (selectedColumn == i);
                if (ImGui::Selectable(runs[0].columnNames[i].c_str(), is_selected))
                    selectedColumn = i;
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
        ImGui::Checkbox("Raw Runs", &showRaw);
        ImGui::Checkbox("Median", &showMedian);
        ImGui::Checkbox("P5", &showP5);
        ImGui::Checkbox("P95", &showP95);
        ImGui::Checkbox("P50", &showP50);
        ImGui::Checkbox("Log Y", &logY);
        ImGui::End();

        ImGui::Begin("Plot");

        if (ImPlot::BeginPlot("Time Series")) {

            auto& stats = columnStats[selectedColumn];
            const auto& colName = runs[0].columnNames[selectedColumn];

            ImPlot::SetupAxes("Time", colName.c_str());
            ImPlot::SetupAxisScale(ImAxis_Y1, logY ? ImPlotScale_Log10 : ImPlotScale_Linear);

            // -------------------------
            // Raw runs
            // -------------------------
            if (showRaw) {
                ImPlot::PushStyleVar(ImPlotStyleVar_LineWeight, 1.0f);
                ImPlot::PushStyleColor(ImPlotCol_Line, ImVec4(0.7f, 0.7f, 0.7f, 0.4f));

                for (size_t r = 0; r < runs.size(); ++r) {
                    ImPlot::PlotLine(
                        ("Run " + std::to_string(r)).c_str(),
                        time.data(),
                        runs[r].columns[selectedColumn].data(),
                        (int) time.size(),
                        ImPlotLegendFlags_None
                    );
                }

                ImPlot::PopStyleColor();
                ImPlot::PopStyleVar();
            }

            // -------------------------
            // Median
            // -------------------------
            if (showMedian) {
                ImPlot::PushStyleVar(ImPlotStyleVar_LineWeight, 3.0f);
                ImPlot::PushStyleColor(ImPlotCol_Line, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));

                ImPlot::PlotLine("Median",
                    time.data(),
                    stats.p50.data(),
                    (int) time.size());

                ImPlot::PopStyleColor();
                ImPlot::PopStyleVar();
            }

            // -------------------------
            // P5
            // -------------------------
            if (showP5) {
                ImPlot::PushStyleColor(ImPlotCol_Line, ImVec4(0.2f, 0.6f, 1.0f, 1.0f));
                ImPlot::PlotLine("P5",
                    time.data(),
                    stats.p5.data(),
                    (int) time.size());
                ImPlot::PopStyleColor();
            }

            // -------------------------
            // P95
            // -------------------------
            if (showP95) {
                ImPlot::PushStyleColor(ImPlotCol_Line, ImVec4(1.0f, 0.3f, 0.3f, 1.0f));
                ImPlot::PlotLine("P95",
                    time.data(),
                    stats.p95.data(),
                    (int) time.size());
                ImPlot::PopStyleColor();
            }

            ImPlot::EndPlot();
        }
        ImGui::End();


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