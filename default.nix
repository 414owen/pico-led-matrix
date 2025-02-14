with import <nixpkgs> {};

let
  pico-sdk = pkgs.fetchgit {
    url = "https://github.com/raspberrypi/pico-sdk.git";
    rev = "2d5789eca89658a7f0a01e2d6010c0f254605d72";
    sha256 = "17cl1kzjiibsl3g27b7mk9g2cy2vwpk56649r8pjgk6bnl3z5h82";
    fetchSubmodules = true;
  };
  picotool = stdenv.mkDerivation {
    name = "picotool";
    version = "0.1";
    isExecutable = true;

    src = fetchFromGitHub {
      owner = "raspberrypi";
      repo = "picotool";
      rev = "c15ac281581318b7e2fb55ff613f71c7bde0a788";
      sha256 = "1d21s5s8wcsrhnw2psgwnhci96kwf2hk92wwgfrp6ffzgah4lx39";
    };

    nativeBuildInputs = [
      cmake
      pkg-config
    ];
    buildInputs = [
      libusb
    ];

    PICO_SDK_PATH = "${pico-sdk}/";

    installPhase = ''
      install -m755 -D picotool $out/bin/picotool
    '';
  };
  retry = pkgs.buildGoPackage {
    pname = "retry";
    version = "1.1.0";

    src = fetchFromGitHub {
      owner = "joshdk";
      repo = "retry";
      rev = "v1.1.0";
      sha256 = "0y8lw1b0nl3crqi3iqx8i6zcgl92vspb8qnsnfb3zsz24kgwy0hf";
    };

    goPackagePath = "github.com/joshdk/retry";
  };
  bootterm = stdenv.mkDerivation {
    name = "bootterm";
    version = "0.3";
    isExecutable = true;

    src = fetchFromGitHub {
      owner = "wtarreau";
      repo = "bootterm";
      rev = "v0.3";
      sha256 = "12aixynf39wafrxvnqn2db3zsk4qf4avpz0pdyczkgld4rr3giw2";
    };

    installPhase = ''
      install -m755 -D bin/bt $out/bin/bt
    '';
  };
in stdenv.mkDerivation rec {
  name = "rpi-pico-template";

  nativeBuildInputs = [
    bash
    bootterm
    retry
    cmake
    gcc-arm-embedded
    screen
    python3
    doxygen
    graphviz
    picotool
  ];

  PICO_SDK_PATH = "${pico-sdk}/";

  src = ./.;

  buildCommand = ''
    cmake $src
    make

    install -D -m 444 -t "$out" main.*
  '';
}
