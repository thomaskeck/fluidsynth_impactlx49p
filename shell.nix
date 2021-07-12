{ pkgs ? import <nixpkgs> {} }:
  pkgs.mkShell {
    nativeBuildInputs = with pkgs; [ buildPackages.gcc buildPackages.fluidsynth buildPackages.fmt gnumake vscode ];
}
