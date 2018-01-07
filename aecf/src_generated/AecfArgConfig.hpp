#pragma once
// frtdruzb20
#include "../src/AecfArguments.hpp"
struct AecfArgConfig {
  bool m_help;
  bool m_output_input_sequences;
  bool m_invert_colors;
  bool m_output_true_color;
  bool m_darker_fg;
  bool m_darker_bg;
  bool m_lighter_fg;
  bool m_lighter_bg;
  bool m_no_bold;
  bool m_no_faint;
  bool m_no_italic;
  bool m_no_underline;
  bool m_no_slow_blink;
  bool m_no_rapid_blink;
  bool m_no_blink;
  bool m_greyify;
  bool m_force_black_white;
  bool m_force_white_black;
  bool m_drop_foreground_color;
  bool m_drop_background_color;
  bool m_toggle_black_white;
};

AecfArgConfig createAecfArgConfig(const AecfArguments & aecfArguments);
