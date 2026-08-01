{ pkgs ? import <nixpkgs> {} }:

let
  inherit (pkgs) lib;
in

pkgs.stdenv.mkDerivation {
  name = "SDL2-PLATFORMER";
  src = ./.;
  nativeBuildInputs = with pkgs; [
    cmake
    pkg-config
  ];
  buildInputs = with pkgs; [
    SDL2
    sdl2-compat
    SDL2_image
    SDL2_mixer
    libwebp
    libtiff
  ];
  meta.mainProgram = "main";
}
