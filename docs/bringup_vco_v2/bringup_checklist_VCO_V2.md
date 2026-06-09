# VCO V2 — Bring-Up & Verification Checklist

Board: `220526_VCO_V2` · Rev: V2 (JLCPCB) · Date: ______ · Ambient: ______ °C
Equipment: Tektronix 2225 (analog, 10× probe — compensate at PRB ADJ first), TOOLTOP ET828 Pro (DMM + Hz/duty). V/oct reference: Fireface + SPAN/Bitwig tuner.

Known issues this revision: wire bridge on −12V rail, wire bridge on R7 network, D4 LED not populated (to be added in V0.2 with dedicated driver off buffered SAW output).

---

## Phase 0 — Unpowered checks (DMM)

| # | Check | Where | Expected | Measured | OK |
|---|-------|-------|----------|----------|----|
| 0.1 | +12V ↔ GND resistance | J1 pin 3 ↔ pin 2 | kΩ-range, climbing (caps) | 190 kΩ climbing | ✓ |
| 0.2 | −12V ↔ GND resistance | J1 pin 1 ↔ pin 2 | kΩ-range, climbing | 190 kΩ climbing | ✓ |
| 0.3 | +12V ↔ −12V resistance | J1 pin 3 ↔ pin 1 | no short | 250 kΩ climbing | ✓ |
| 0.4 | −12V bridge continuity | J1 pin 1 → D3-K → R21 → U2 pin 11, C7, C4, C9 | beep, stable under wiggle | ✓ | ✓ |
| 0.5 | R7 bridge continuity | −12V rail → R7 pad 1 → COARSE1 pad 3 | beep, stable under wiggle | ✓ | ✓ |
| 0.6 | GND integrity | J1 pin 2 → U1 pin 7, U2 GND refs, all jack sleeves | beep | ✓ | ✓ |
| 0.7 | D2 diode check (fwd) | A → K | ~0.2–0.4 V | 0.193 V | ✓ |
| 0.8 | D3 diode check (fwd) | A → K | ~0.2–0.4 V | 0.193 V | ✓ |

> Note: D2/D3 are SB140 Schottky — 0.193 V forward drop is correct for this type.

## Phase 1 — First power, rails only (DMM, no patch cables)

| # | Check | Where | Expected | Measured | OK |
|---|-------|-------|----------|----------|----|
| 1.1 | +12V rail | U2 pin 4 vs GND | ~+11.4…+11.6 V | +11.83 V | ✓ |
| 1.2 | −12V rail | U2 pin 11 vs GND | ~−11.4…−11.6 V | −11.92 V | ✓ |
| 1.3 | −12V at far end of bridge | C9 pad 2 vs GND | = 1.2 ± 10 mV | −11.92 V | ✓ |
| 1.4 | U1 supply | U1 pin 14 vs pin 7 | ~+11.5 V | +11.83 V | ✓ |
| 1.5 | Drop across R20 | both pads | < 0.3 V | 23.6 mV | ✓ |
| 1.6 | Drop across R21 | both pads | < 0.3 V | 92 mV | ✓ * |
| 1.7 | Thermal check | all ICs, transistors, 1 min | nothing warm | pass | ✓ |

> \* 1.6 asymmetry vs 1.5 (factor ~4): attributable to TL074 output-stage sink current returning via V−. Both values well within acceptable range. Monitor in V0.2 with matched layout.
> Note: 1.3 initially read 337 mV due to probing wrong pad. Re-measured correctly at C9 pad 2 = −11.92 V, confirmed by resistance check U2 pin 11 ↔ C9 pad 2 = 0.4 Ω. No fault.

## Phase 2 — DC operating points (DMM)

| # | Node | Where to probe | Expected | Measured | OK |
|---|------|----------------|----------|----------|----|
| 2.1 | COARSE ref range (R7 network) | COARSE1 pad 3 | −6…−9 V (load-dependent) | −6.8 … −8.6 V | ✓ |
| 2.2 | COARSE top ref | COARSE1 pad 1 | ~+1.5…+2.5 V | +2.4 … +1.01 V | ✓ |
| 2.3 | FINE bottom ref | FINE1 pad 3 | ≈ −6 V | −5.7 V | ✓ |
| 2.4 | CV summing node | Q2 base (pad 2) vs GND | sub-volt, moves with COARSE | 19.5 mV | ✓ |
| 2.5 | Expo drive | Q1 base (pad 2) vs GND | ~0.35–0.6 V while oscillating | 512 mV | ✓ |
| 2.6 | LED node | D4 | not populated in this revision | — | — |

> Note: 2.1/2.2 vary with wiper position due to loading of CV summing network — correct behaviour. R7 bridge confirmed functional via voltage range and 0.4 Ω continuity check.

## Phase 3 — Signal chain (scope, 10×, DC coupling unless noted)

Probe ground to any jack sleeve or J1 pin 2. Tek 2225 (analog): document each row with an iPhone photo of the CRT. Amplitude = divisions × V/div × 10 (probe). V/div and s/div settings read from scope panel photos.

| # | Stage | Probe at | V/div (panel) | s/div | Vpp (calculated) | f (calculated) | Photo refs | OK |
|---|-------|----------|--------------|-------|-----------------|----------------|------------|----|
| 3.1 | Saw core | C1 pad 1 (= D1-K, Q1-C, U1 pin 1) | 1V (×10=10V) | 0.5ms | ~8 Vpp | ~800 Hz | IMG_1948 (probe), IMG_1949 (CRT), IMG_1950 (settings) | ✓ |
| 3.2 | Core buffer | U2 pin 1 | — | — | = 3.1 | = 3.1 | (not separately photographed — identical to 3.1) | ✓ |
| 3.3 | AC-coupled saw | U2 pin 5 | 1V (×10=10V) | 0.5ms | ~8 Vpp, 0V centered | ~800 Hz | IMG_1952 | ✓ |
| 3.4 | Amplified saw | U2 pin 7 | 1V (×10=10V) | 0.5ms | ~9–10 Vpp | ~800 Hz | IMG_1953 (probe), IMG_1954 (CRT) | ✓ |
| 3.5 | SAW output | SAW_OUT1 tip (after R13) | 1V (×10=10V) | 0.5ms | ~9–10 Vpp | ~800 Hz | IMG_1974 (CRT) | ✓ |
| 3.6 | Comparator out | U2 pin 8 | 5V (×10=50V) | 0.2ms | ~21–22 Vpp | ~800 Hz | IMG_1955 (probe), IMG_1960 (CRT) | ✓ |
| 3.7 | Pulse output | PULSE_OUT1 tip (after R16) | 5V (×10=50V) | 0.2ms | 8.88 Vpp | 1.65 kHz | IMG_1965 (ET828) | ✓ |
| 3.8 | PWM sweep | U2 pin 8 / PULSE_OUT while turning PWM1 | 5V | 0.2ms | — | — | IMG_1962 (~30% duty), IMG_1960 (~50%), IMG_1978 (~70%), IMG_1976 (PWM=0, latched) | ✓ |
| 3.9 | Saw reset edge | C1 pad 1, fast timebase | — | — | small overshoot at top, clean fall | — | IMG_1966 (ET828 AC, shark-fin at low f) | ✓ * |
| 3.10 | Frequency range | ET828 Hz on PULSE_OUT, COARSE min→max | — | — | — | f_min ≈ 5.19 Hz / f_max ≈ 49.8 kHz | IMG_1966 (f_min), IMG_1968/1969/1972 (f_max) | ✓ |

> \* 3.9 Reset edge: small overshoot spike visible at reset point on Tek (IMG_1974 top of ramp). Normal for TL074 output stage. No sustained ringing observed. AC-coupled shot at 5 Hz shows shark-fin distortion — expected, capacitor discharge effect at sub-audio frequencies, not a circuit fault.
> Note: ET828 AC-mode Vpp at 5 Hz (14.4 Vpp) is unreliable — well outside the instrument's linear frequency response. Tek measurement (DC-coupled, ~9–10 Vpp) is authoritative.

## Phase 4 — V/oct verification

Feed known CVs into CV_IN1 (MCP4728 chain or precision source). Measure frequency via Fireface + Voxengo SPAN (or Bitwig tuner) — far more accurate than graticule counting on the 2225. ET828 Hz mode (~±1 % rdg ≈ tens of cents) only for f_min/f_max and relative drift, not the cents table.

| CV (V) | f expected (Hz) | f measured (Hz) | Error (cents) |
|--------|-----------------|-----------------|---------------|
| 0 | | | |
| 1 | | | |
| 2 | | | |
| 3 | | | |
| 4 | | | |

- ADJ_TUNE1: set and locked. Tracking verified over 2 octaves via Fireface + Voxengo SPAN.
- **Result: ~10 cents total over 2 octaves (≈5 ct/oct)** — better than breadboard baseline (~14 ct/oct). Remaining error to be handled by STM32 autotune (Timer Input Capture → DAC correction). SPAN screenshots pending.
- Drift test: fixed CV, log f at t=0 / 5 / 10 / 20 min → tempco data for BC847BS redesign justification.

| t (min) | f (Hz) | Ambient (°C) |
|---------|--------|--------------|
| 0 | | |
| 5 | | |
| 10 | | |
| 20 | | |

## Phase 5 — Known issues / revision notes

- [x] −12V bridge: continuity confirmed (0.4 Ω, U2 pin 11 ↔ C9 pad 2). Photos: IMG_1951 (PCB back / solder side).
- [x] R7 bridge: COARSE1 pad 3 voltage range −6.8…−8.6 V confirms function.
- [x] Frequency range confirmed: f_min ≈ 5 Hz, f_max ≈ 49.8 kHz. Full usable audio range covered.
- [x] PWM sweep: full duty range functional, latches only at PWM=0 (threshold outside ±1.5V window — correct behaviour).
- [ ] D4 LED: not populated. V0.2: drive from buffered SAW output with series resistor, remove R22 from CV summing node.
- [ ] Expo converter: NPN/PNP (Q1/Q2) drift data to be collected in Phase 4. V0.2: monolithic matched pair (BC847BS) + tempco resistor.
- [x] V/oct tracking trimmed: ~10 ct over 2 octaves, SPAN-verified. Photos pending.
- [ ] Drift test (tempco data for BC847BS redesign) pending
- [ ] Flux cleaned around Q1/Q2/thermistor area (TH1–TH4)

---

**Sign-off:** Board functional ✓ · Calibrated ✓ (10 ct / 2 oct) · Documented ☐ (SPAN photos pending)
