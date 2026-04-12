#!/bin/bash

PROGRAM="./build/AplikacjaPEA"
CONFIGS_DIR="configs/plikowe"

for config in "$CONFIGS_DIR"/*.toml; do
    echo "Uruchamiam: $config"
    $PROGRAM "$config"
    echo "Zakończono: $config"
    echo "--------------------------"
done

echo "Wszystkie konfiguracje wykonane!"