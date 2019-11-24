#include "pch.hpp"
#include <sstream>
#include <viperfish_dear_imgui.hpp>
#include <viperfish_stats_window.hpp>
#include <fluxions_gte_noise.hpp>

namespace Vf
{

static Fluxions::Noise noise;

FloatStat::FloatStat(int max_samples, float metric_min, float metric_max) {
	metric_len = max_samples;
	metric_max_len = 4 * max_samples;
	metric.resize(max_samples, metric_init_value);
	metric_min_value = metric_min;
	metric_max_value = metric_max;
}

void FloatStat::push(float x) {
	metric_min_value = std::min(metric_min_value, x);
	metric_max_value = std::max(metric_max_value, x);
	metric.push_back(x);
	if (metric.size() > metric_max_len) {
		int chop = metric_max_len >> 1;
		metric.erase(metric.begin(), metric.begin() + chop);
	}
	metric_pl_size = std::min((int)metric.size(), metric_len);
	metric_pl_start = (int)metric.size() - metric_pl_size;
}

void FloatStat::push_difference(float x) {
	push(x - metric_last_value);
	metric_last_value = x;
}

void FloatStat::push_difference_inverse(float x) {
	float denom = x - metric_last_value;
	if (fabsf(denom) < FLT_EPSILON)
		push(0.0f);
	else
		push(1.0f / denom);
	metric_last_value = x;
}

void FloatStat::plotLines(const char* label) {
	if (!visible)
		return;

	ImGui::PlotLines(label != nullptr ? label : "",
					 &metric[metric_pl_start],
					 metric_pl_size,
					 0, (const char*)0,
					 metric_min_value, metric_max_value,
					 ImVec2(0.0f, height));
}

float FloatStat::get(int idx) const {
	return metric[metric_pl_start + idx];
}

void FloatStat::set(int idx, float x) {
	metric[metric_pl_start + idx] = x;
}

StatsWindow::StatsWindow(const std::string& name)
	: Window(name) {
	std::ostringstream ostr;
	ostr << name << (void*)this;
	popupId = ostr.str();

	float_stats["et"].visible = false;
}

StatsWindow::~StatsWindow() {}

void StatsWindow::OnUpdate(double timeStamp) {
	Window::OnUpdate(timeStamp);
	if (pause) {
		auto& et = float_stats["et"];
		auto& frac = float_stats["3frac"];
		auto& turb = float_stats["4turb"];
		for (int i = 0; i < MAX_SAMPLES; i++) {
			float t = et.get(i);
			frac.set(i, noise.fractal1(t, min_freq, max_freq));
			turb.set(i, noise.turbulence1(t, min_freq, max_freq));
		}
		return;
	}
	float_stats["et"].push(timeStamp);
	float_stats["dt"].push_difference(timeStamp);
	float_stats["fps"].push_difference_inverse(timeStamp);
	float_stats["sin"].push(sin(timeStamp));
	float_stats["1noise"].push(noise.noise((float)timeStamp));
	float_stats["2snoise"].push(noise.snoise((float)timeStamp));
	float_stats["3frac"].push(noise.fractal1((float)timeStamp, min_freq, max_freq));
	float_stats["4turb"].push(noise.turbulence1((float)timeStamp, min_freq, max_freq));
	float_stats["t"].push(timeStamp - int(timeStamp));
}

void StatsWindow::OnRenderDearImGui() {
	HFLOGDEBUGFIRSTRUNCOUNT(MAX_RUN_MESSAGES);
	if (!beginWindow()) return;
	Window::OnRenderDearImGui();

	for (auto& fs : float_stats) {
		fs.second.plotLines(fs.first);
	}

	ImGui::SliderFloat("Fmin", &min_freq, 1.0f, 8.0f);
	ImGui::SliderFloat("Fmax", &max_freq, 1.0f, 16.0f);

	if (!pause && ImGui::Button("Pause")) {
		pause = true;
	}

	if (pause && ImGui::Button("Resume")) {
		pause = false;
	}

	min_freq = floorf(min_freq);
	max_freq = floorf(max_freq);

	endWindow();
}

} // namespace Vf