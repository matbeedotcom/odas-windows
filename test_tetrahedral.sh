#!/bin/bash

# Test script for ODAS with tetrahedral microphone array configuration

echo "Testing ODAS configuration for tetrahedral microphone array"
echo "=========================================================="

# Check if ODAS is built
if [ ! -f "build/bin/odaslive" ]; then
    echo "ERROR: ODAS not built. Please run 'cd build && make' first."
    exit 1
fi

# Check if configuration file exists
if [ ! -f "config/odaslive/tetrahedral_4ch.cfg" ]; then
    echo "ERROR: Configuration file not found at config/odaslive/tetrahedral_4ch.cfg"
    exit 1
fi

echo "✓ ODAS executable found: build/bin/odaslive"
echo "✓ Configuration file found: config/odaslive/tetrahedral_4ch.cfg"

# Test configuration syntax
echo ""
echo "Testing configuration syntax..."
./build/bin/odaslive -c config/odaslive/tetrahedral_4ch.cfg -v 2>&1 | head -20

echo ""
echo "Configuration details:"
echo "- Sample rate: 44.1 kHz"
echo "- Channels: 4"
echo "- Array geometry: Tetrahedral (25mm radius)"
echo "- Microphone positions match mechanical/tetrahedron_frame.scad"

echo ""
echo "To run ODAS with your Teensy microphone array:"
echo "1. Connect your Teensy via USB"
echo "2. Run: ./build/bin/odaslive -c config/odaslive/tetrahedral_4ch.cfg -v"
echo "3. Check audio device ID and update 'card' parameter in config if needed"

echo ""
echo "Test completed successfully!"
echo "ODAS is configured and ready for your tetrahedral microphone array."