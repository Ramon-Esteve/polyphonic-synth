# DIY Polyphonic Analog Synthesizer

A university project building a polyphonic analog synthesizer from scratch using [mki x es.EDU](https://www.ericasynths.lv/shop/diy-kits-1/mki-x-esedu-diy-system/) module designs and an STM32 Nucleo board as a MIDI-to-CV controller.

The goal is to go from a single voice on a breadboard to a playable multi-voice analog synth controlled via MIDI — while documenting every step of the process.

## What This Project Is

This is an educational build that combines:
- **Analog circuit design** — VCO, VCF, VCA and envelope circuits based on the publicly available Erica Synths / Moritz Klein educational module designs
- **Embedded firmware** — STM32 Nucleo running custom MIDI-to-CV firmware (MIDI parsing, DAC output, voice allocation)
- **System integration** — bridging the digital (MIDI) and analog (CV/gate) worlds to create a playable instrument

All analog circuits are prototyped on breadboard. The Nucleo board handles MIDI input and outputs control voltages (1V/oct pitch CV, gate, and additional CV for filter/PWM) through its DAC and an op-amp scaling stage.

## Current Status

> **Phase 1 — VCO + MIDI pitch control** *(in progress)*

- [x] VCO built and working on breadboard
- [ ] Nucleo DAC output verified
- [ ] Op-amp CV scaling circuit (0–3.3V → 0–5V)
- [ ] MIDI input (USB or UART @ 31250 baud)
- [ ] MIDI note-to-CV conversion with 1V/oct tracking
- [ ] Gate output
- [ ] Calibration and tuning

## Build Phases

| Phase | Module | Description | Status |
|-------|--------|-------------|--------|
| 1 | VCO + MIDI-to-CV | Pitch control of a single oscillator via MIDI keyboard | In progress |
| 2 | VCA + Envelope | Amplitude control with gate/envelope from Nucleo | Planned |
| 3 | VCF | Voltage-controlled filter with CV control of cutoff | Planned |
| 4 | Polyphony | Duplicate voice chains, multi-channel DAC, voice allocation | Planned |

## Hardware

- **Microcontroller:** STM32 Nucleo board (<!-- replace with your specific model, e.g. Nucleo-F446RE -->)
- **Analog modules:** Based on mki x es.EDU circuit designs (VCO, VCA, VCF, Envelope)
- **Prototyping:** Breadboard (switched from stripboard after killing two ICs soldering — lesson learned)
- **DAC output stage:** Op-amp scaling circuit to convert Nucleo's 0–3.3V DAC to 0–5V+ for 1V/oct

## Repository Structure

```
├── docs/                   # Build documentation for each module
├── firmware/               # STM32CubeIDE project — MIDI-to-CV firmware
├── schematics/             # Circuit diagrams and breadboard layouts
├── measurements/           # Tuning data, oscilloscope captures, test results
├── media/                  # Build photos, video material, audio samples
└── references/             # Datasheets, links, related projects
```

## Documentation

Each build phase is documented with:
- Circuit description and design rationale
- Breadboard layout and build notes
- Test procedures and measurements
- Problems encountered and how they were solved

Video documentation accompanies the written docs — see the [media/](media/) folder or the <!-- add link to your YouTube/channel here --> video series.

## Key References

- [mki x es.EDU module manuals](https://www.ericasynths.lv/shop/diy-kits-1/mki-x-esedu-diy-system/) — free PDFs covering circuit design from scratch
- [Moritz Klein's YouTube channel](https://www.youtube.com/@MoritzKlein0) — companion videos to the EDU modules
- [ModSynth MIDI2CV](https://warrantyvoids.github.io/ModSynth/modules/midi2cv/) — STM32 Nucleo-based MIDI-to-CV reference project
- [STM32 DAC getting started](https://wiki.st.com/stm32mcu/wiki/Getting_started_with_DAC) — official ST tutorial

## License

This project is licensed under the MIT License — see [LICENSE](LICENSE) for details.

Note: The analog circuit designs are based on the mki x es.EDU educational series by Erica Synths and Moritz Klein. Their manuals are freely available but the original designs remain their work. This repository contains my own implementation, firmware, documentation, and modifications.
