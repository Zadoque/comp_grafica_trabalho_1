{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  packages = with pkgs; [
    bash
    gcc
    gnumake
    freeglut
    libGL
    libGLU
  ];

  LD_LIBRARY_PATH = "/run/opengl-driver/lib:/run/opengl-driver-32/lib";
}