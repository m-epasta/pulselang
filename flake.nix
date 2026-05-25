{
  description = "nix-shell for pulselang development";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/25.11";
  };

  outputs = {
    self,
    nixpkgs,
  }: let
    system = "x86_64-linux";
    pkgs = nixpkgs.legacyPackages.${system};
  in {
    devShells.${system}.default = pkgs.mkShell {
      buildInputs = with pkgs; [
        clang-tools
        gcc
        glibc.dev
        tinycc
        gnumake
        # libc
      ];

      shellHook = ''
        export C_INCLUDE_PATH=${pkgs.glibc.dev}/include
        export CLANGD_FLAGS="--query-driver=${pkgs.gcc}/bin/gcc"
      '';
    };
  };
}
