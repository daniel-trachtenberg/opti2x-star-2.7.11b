# Wave 3 summary — STAR 2.7.11b serial-align-primary

**When:** 2026-09-09 ~6:13–6:35 PM PT  
**Parent binary:** `experiments/candidate-wave2/STAR` (~1.594× vs baseline; median ~1.38–1.39 s)  
**Frozen baseline median (Wave3 paired):** 2.22 s  
**Oracle sorted SAM body:** `20739c12f887bbf5b5eba10c29d4546ac8f4fb3d9996079ccc377162abbce483`  
**NLWP:** 1 on all spot-checks. `--runThreadN 1` only. No OpenMP thread bump. No `bgzf_mt`.  
**No false QUALIFIED.** Correct rejection preferred over fake win.

## Candidate-wave2 reprofile

See `profiles/PROFILE_candidate_wave2.md`. Top exclusive:

| Rank | Symbol | Excl % | Amdahl |
|-----:|--------|-------:|-------:|
| 1 | stitchAlignToTranscript | 24.29 | ~1.32 |
| 2 | extendAlign | 12.57 | ~1.14 |
| 3 | stitchWindowAligns | 11.80 | ~1.13 |
| 4 | compareSeqToGenome | 9.66 | ~1.11 |
| 5 | assignAlignToWindow | 5.90 | ~1.06 |
| 6 | copyStitchCore | 5.67 | ~1.06 |

`operator=` no longer in top (H8+H13). Residual: stitch/extend/SJ scan + SA compare + fork memcpy.

## Rough combined Amdahl (toward 2×)

Define remaining **stitch+extend+assign** exclusive bundle ≈  
`stitchAlign` 24.29% + `extendAlign` 12.57% + `copyStitchCore` 5.67% + `assignAlignToWindow` 5.90% ≈ **48.4%**.

- If that bundle were **halved** (remove 24.2% of wall): candidate speedup S≈**1.32×** → e2e vs baseline ≈1.594×1.32 ≈ **2.10×** — **plausible in theory**.
- Need **≈1.255×** more from candidate-wave2 to hit 2.0× ⇒ must remove ≈**20.3%** of wall ≈ **42%** of that bundle.
- Including `stitchWindowAligns` self (bundle≈60%): half → S≈1.43 → e2e≈**2.28×** theoretical.
- **Empirical Wave3:** no isolated change removed enough wall under oracle+serial constraints to clear the **≥1.03× vs parent** gate. Theoretical ceilings ≠ achieved speedups.

## Decisions (isolated vs live parent; gate ≥~1.03×)

| ID | Decision | approx speedup vs parent | Notes |
|----|----------|-------------------------:|-------|
| H15_stitch_closed_form_motif | **rejected** | ≤1.00× (often slower) | Closed-form Score+= + packed SJ motif; oracle PASS; no win (echoes H3/H9) |
| H16_extendAlign_specialize | **rejected** | ≤1.00× | ±1 pointer walks; oracle PASS; neutral/regress |
| H17_stitch_core_ctor | **rejected** | ~1.02× | StitchCoreTag ctor skips reset(); below gate |
| H18_wTr_stitch_core_assign | **rejected** | ~1.00× | copyStitchCore+clear for wTr record; oracle PASS |
| H19_compareSeq_block8 | **rejected** | ~0.99× | 8-byte block compare fwd/fwd; oracle PASS |
| H20_stitch_fork_pool | **rejected** | **~1.015×** | Recursive Transcript pool; best near-miss; below gate |
| H21_extend_int_budget | **rejected** | ~0.98× | Pointer-specialize non-e2e loop; slight regress |
| H22_lto_native | **rejected** | ~0.97× | `-march=native` only (LTO broke htslib link); regress |
| H23_pgo | **rejected** | ~0.92× | `-fprofile-generate/use` on fixture; oracle PASS; slower |
| H24_transcript_pod_layout | **rejected** | ~0.94× | Contiguous POD + full-prefix memcpy; worse than partial exon copy |
| H25_funroll_inline | **rejected** | ~0.96× | `-funroll-loops -finline-limit=2000`; regress |

**Accepted Wave3 patches: none.**

## Cumulative combination

- Artifact: `experiments/candidate-wave3/` (= wave2 binary + patch set)
- Paired 9-round median speedup vs baseline: **≈1.597×** (base med 2.22 s → cand med 1.39 s)
- Still **not** ≥2.0× (need ≈1.252× more). No N=30. **Not QUALIFIED.**
- Status package: `deliverable-NOT_YET/`

## Honest gap + why 2× not established

- Best established serial+oracle speedup on this fixture: **~1.59–1.60×**.
- Remaining hot exclusive mass is **necessary per-base stitch/extend/SA work** plus **exponential include/exclude stitch recursion** (`stitchWindowAligns` ~63% inclusive).
- Closing ~1.25× more would require removing ~20% of wall without threads, without weaker validation, and without changing alignment scores/outputs — Wave3 micros that preserve oracle did not clear 1.03× parent gate.
- Forbidden shortcuts that could fake 2×: `bgzf_mt` / OpenMP bump / `--runThreadN>1` / oracle edits / CRC skip / cross-run result cache.

## Next experiments (if another wave)

1. True **POD StitchTr** type (no `std::vector`/`set` in recursion) + convert only at `wTr` record — larger than H20/H24.
2. **Transactional stitchAlign** (compute then commit) enabling mutate/undo forks with smaller deltas than full `copyStitchCore`.
3. **Search-tree pruning** that is proven score-equivalent (hard; high risk to oracle).
4. SA path: only if still ≥~10% after (1)–(2); careful identity-preserving block compare for reverse dirs.
5. Do **not** claim QUALIFIED without ≥2.0 median + N=30 lower_95 under contract.

## Paths
- `profiles/PROFILE_candidate_wave2.md` (+ perf-candidate-wave2-*.txt/.data)
- `hypotheses/hypotheses.jsonl`
- `experiments/H15_*` … `H25_*` (all rejected)
- `experiments/candidate-wave3/{STAR,combined.patch,COMPARE.md,timing/}`
- `deliverable-NOT_YET/`
- `experiments/WAVE3_SUMMARY.md` (this file)
