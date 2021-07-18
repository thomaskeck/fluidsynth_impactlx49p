# Fluidsynth for ImpactLX49+

During quarantine I bought an Impact LX49+ midi keyboard
(see https://www.thomann.de/de/nektar_impact_lx49_395176.htm).

In this project I implemented support for fluidsynth for this keyboard from scratch.
Most likely the code will work for other midi instruments as well.

## NixOS Software Setup

I used NixOS to deal with the software stack. I can recommend using pipewire in order to have a unified setup for alsa, pulseaudio and jack.

```
{ config, pkgs, ... }:
{
  # rtkit is optional but recommended
  security.rtkit.enable = true;
  services.pipewire = {
    enable = true;
    alsa.enable = true;
    alsa.support32Bit = true;
    pulse.enable = true;
    jack.enable = true;
  };

  environment.systemPackages = with pkgs; [
    pavucontrol
    qjackctl
    ardour
    hydrogen
    fluidsynth
    alsaUtils
  ];

  security.pam.loginLimits = [
    { domain = "@audio"; item = "memlock"; type = "-"; value = "unlimited"; }
    { domain = "@audio"; item = "rtprio"; type = "-"; value = "99"; }
    { domain = "@audio"; item = "nofile"; type = "soft"; value = "99999"; }
    { domain = "@audio"; item = "nofile"; type = "hard"; value = "99999"; }
  ];
}
```
