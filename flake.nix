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
        # NOTE: Consider setting some programs like your editor into systemPackacges so you can access them while having a *lightweight* nix-shell

        clang-tools
        gcc
        glibc.dev
        tinycc
        gnumake
        valgrind-light
        # libc

        # git utilities
        gh
        git
      ];

      shellHook = ''
        export C_INCLUDE_PATH=${pkgs.glibc.dev}/include
        export CLANGD_FLAGS="--query-driver=${pkgs.gcc}/bin/gcc"
      '';
    };
  };
}
