#ifndef VIPERFISH_STATS_WINDOW_HPP
#define VIPERFISH_STATS_WINDOW_HPP

#include <viperfish_window.hpp>

namespace Vf
{
	class FloatStat {
	public:
		FloatStat(int max_samples = 100, float metric_low = FLT_MIN, float metric_high = FLT_MAX);
		void push(float value);
		inline void push(double value) { push((float)value); }
		void push_difference(float value);
		inline void push_difference(double value) { push_difference((float)value); }
		void push_difference_inverse(float value);
		inline void push_difference_inverse(double value) { push_difference_inverse((float)value); }
		void plotLines(const char* label);
		inline void plotLines(const std::string& label) { plotLines(label.c_str()); }

		float get(int idx) const;
		void set(int idx, float x);

		bool visible = true;
		float height = 0.0f;
	private:
		std::vector<float> metric{ 0.0f };
		int metric_pl_start = 0;
		int metric_pl_size = 0;
		int metric_idx = 0;
		int metric_len = 100;
		int metric_max_len = 1000;
		float metric_init_value = 0.0f;
		float metric_last_value = 0.0f;
		float metric_min_value = 0.0f;// FLT_MIN;
		float metric_max_value = 1.0f;// FLT_MAX;
	};

	class StatsWindow : public Window {
	public:
		StatsWindow(const std::string& name);
		virtual ~StatsWindow() override;

		void OnUpdate(double timeStamp) override;
		void OnRenderDearImGui() override;

		std::string msg;
	private:
		std::string popupId;
		static constexpr int MAX_SAMPLES = 256;
		std::map<std::string, FloatStat> float_stats{
			{ "et",      { MAX_SAMPLES,  0.0f,   0.5f } },
			{ "dt",      { MAX_SAMPLES,  0.0f,   0.5f } },
			{ "fps",     { MAX_SAMPLES,  0.0f, 100.0f } },
			{ "sin",     { MAX_SAMPLES, -1.0f,   1.0f } },
			{ "1noise",  { MAX_SAMPLES,  0.0f,   1.0f } },
			{ "2snoise", { MAX_SAMPLES, -1.0f,   1.0f } },
			{ "3frac",   { MAX_SAMPLES, -1.0f,   1.0f } },
			{ "4turb",   { MAX_SAMPLES, -1.0f,   1.0f } },
		};
		float min_freq = 1.0f;
		float max_freq = 8.0f;
		bool pause = false;
	};

	using StatsWindowPtr = std::shared_ptr<StatsWindow>;
}

#endif