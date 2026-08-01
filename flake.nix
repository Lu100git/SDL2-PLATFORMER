{
  inputs.nixpkgs.url = "github:nixos/nixpkgs/nixos-26.05";
  inputs.flake-parts.url = "github:hercules-ci/flake-parts";
  inputs.systems.url = "github:nix-systems/default";

  outputs = { self, flake-parts, systems, ... }@inputs: flake-parts.lib.mkFlake { inherit inputs; } {
    systems = import systems;
    perSystem = { pkgs, lib, ... }: {
      packages.default = import ./default.nix { inherit pkgs; };
    };
  };
}
