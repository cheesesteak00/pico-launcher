{
  description = "Pico Launcher Build Environment";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-24.11";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
      in {
        devShells.default = pkgs.mkShell {
          buildInputs = with pkgs; [
            gnumake
            git

            docker
          ];

          shellHook = ''
            echo "Pico Launcher Build Environment"
            echo ""
            echo "Quick start:"
            echo "  ./build.sh               Build LAUNCHER.nds (requires Docker)"
            echo ""
            echo "Note: The actual NDS build runs inside the official BlocksDS"
            echo "      Docker image (skylyrac/blocksds:slim-latest)."
            echo ""
            echo "Build output: LAUNCHER.nds"
          '';
        };
      });
}
