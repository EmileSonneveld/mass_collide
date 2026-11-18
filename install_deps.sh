#!/usr/bin/env bash
set -euxo pipefail

sudo apt-get update
sudo apt-get install -y \
	build-essential \
	cmake \
	libgl1-mesa-dev \
	libglu1-mesa-dev \
	xorg-dev
