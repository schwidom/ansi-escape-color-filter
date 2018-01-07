// 2z3wzi9qjn
#include "AecfArgConfig.hpp"
AecfArgConfig createAecfArgConfig(const AecfArguments & aecfArguments){
return
AecfArgConfig{
aecfArguments.hasOption("--help")
,aecfArguments.hasOption("--output-input-sequences")
,aecfArguments.hasOption("--invert-colors")
,aecfArguments.hasOption("--output-true-color")
,aecfArguments.hasOption("--darker-fg")
,aecfArguments.hasOption("--darker-bg")
,aecfArguments.hasOption("--lighter-fg")
,aecfArguments.hasOption("--lighter-bg")
,aecfArguments.hasOption("--no-bold")
,aecfArguments.hasOption("--no-faint")
,aecfArguments.hasOption("--no-italic")
,aecfArguments.hasOption("--no-underline")
,aecfArguments.hasOption("--no-slow-blink")
,aecfArguments.hasOption("--no-rapid-blink")
,aecfArguments.hasOption("--no-blink")
,aecfArguments.hasOption("--greyify")
,aecfArguments.hasOption("--force-black-white")
,aecfArguments.hasOption("--force-white-black")
,aecfArguments.hasOption("--drop-foreground-color")
,aecfArguments.hasOption("--drop-background-color")
,aecfArguments.hasOption("--toggle-black-white")
};
}
