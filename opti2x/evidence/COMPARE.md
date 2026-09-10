# candidate-wave3 vs baseline — paired compare

**When:** 2026-09-09 ~6:32 PM PT  
**Contract:** `star-2.7.11b-serial-align-fixture-v1`  
**Baseline binary:** `baseline/STAR`  
**Candidate binary:** `experiments/candidate-wave3/STAR` (= wave2; **no Wave3 patch accepted**)  
**sha256:** `010ef3f5dd71f3d283519dc14b4bc70b02598cbd0ce38a783661b2c33ce466d4`  
**Patches:** Wave1 H1+H2 + Wave2 H8+H10+H13 only  
**Argv:** frozen development.yaml (`--runThreadN 1`, `NoSharedMemory`, BAM Unsorted)  
**NLWP:** 1 (`timing/nlwp.txt`)  
**Oracle:** sorted SAM body `20739c12…abbce483` — PASS all 9 paired rounds  
**No 2× claim. No bgzf_mt. Not QUALIFIED.**

## Paired walls (serialize: baseline then candidate each round)

| Round | Baseline s | Candidate s | Speedup |
|------:|-----------:|------------:|--------:|
| 1 | 2.25 | 1.43 | 1.573× |
| 2 | 2.20 | 1.40 | 1.571× |
| 3 | 2.22 | 1.37 | 1.620× |
| 4 | 2.26 | 1.40 | 1.614× |
| 5 | 2.24 | 1.39 | 1.612× |
| 6 | 2.21 | 1.40 | 1.579× |
| 7 | 2.18 | 1.39 | 1.568× |
| 8 | 2.21 | 1.39 | 1.590× |
| 9 | 2.34 | 1.37 | 1.708× |

- **Baseline median:** 2.22 s  
- **Candidate median:** 1.39 s  
- **Median speedup (med/med):** **1.597×**  
- **Median paired speedup:** 1.590×  
- **Geometric mean paired speedup:** 1.603×  
- **Gap to 2×:** need additional ≈1.252× from this candidate  
- **≥2.0×?** **No** — skip N=30 / lower_95; do not self-QUALIFIED.

## Artifacts
- `timing/paired.jsonl`, `timing/COMPARE.json`, `timing/nlwp.txt`
- `combined.patch` (identical to wave2)
