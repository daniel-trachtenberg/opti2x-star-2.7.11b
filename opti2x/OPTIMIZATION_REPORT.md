# OPTIMIZATION REPORT — STAR 2.7.11b serial-align-primary

**Status: NOT_YET**  
**Date:** 2026-09-09 (PT)  
**Contract:** `star-2.7.11b-serial-align-fixture-v1`  
**Claim:** 2× speedup on the frozen development fixture under serial resource envelope is **not established**.

## Executive summary

After three optimization waves, the best oracle-passing serial candidate achieves approximately **1.60×** median wall speedup versus the frozen baseline on `serial-align-primary`. Wave 3 reprofiled `candidate-wave2`, documented an Amdahl path that makes 2× *theoretically* plausible if ~half of remaining stitch/extend/assign exclusive time were removed, then ran profile-driven experiments (H15–H25). **None cleared the ≥1.03× vs-parent acceptance gate.** `candidate-wave3` therefore carries the Wave1+Wave2 patch set only. No N=30 / lower_95 campaign was run. **No QUALIFIED claim.**

## Resource / correctness guards (met)

| Guard | Result |
|-------|--------|
| `--runThreadN 1` | always |
| NLWP | 1 (spot-checks) |
| No `bgzf_mt` / no OpenMP thread bump | confirmed |
| Oracle sorted SAM body | PASS (all Wave3 timing / paired rounds) |
| No oracle / fixture edits | confirmed |
| Serialize timing | paired baseline→candidate |

## Profile tops (candidate-wave2 reprofile)

| Rank | Symbol | Exclusive % |
|-----:|--------|------------:|
| 1 | `stitchAlignToTranscript` | 24.29 |
| 2 | `extendAlign` | 12.57 |
| 3 | `stitchWindowAligns` | 11.80 |
| 4 | `compareSeqToGenome` | 9.66 |
| 5 | `assignAlignToWindow` | 5.90 |
| 6 | `copyStitchCore` | 5.67 |

See `evidence/PROFILE_candidate_wave2.md`.

## Amdahl (Wave3)

- Bundle stitch+extend+copyStitchCore+assign ≈ **48.4%** exclusive.
- Halving that bundle ⇒ ~1.32× on candidate ⇒ ~**2.10×** e2e theoretical vs baseline.
- Need ~**1.25×** more from the ~1.60× candidate to reach 2.0× (must remove ~20% of wall).
- Wave3 did not achieve that fraction under oracle-preserving serial edits.

## Decisions

### Accepted (prior waves; still in best binary)
- H1 Transcript by-ref in `stitchWindowAligns`
- H2 partial exon/SJ Transcript copy
- H8 skip-empty container assign + memcpy used slots
- H10 `extendAlign` mmBreak hoist
- H13 `copyStitchCore` for include forks

### Rejected Wave3 (oracle PASS unless noted; all <1.03× or regress)
- H15 closed-form Score + packed SJ motif
- H16 specialize `extendAlign` ±1 walks
- H17 StitchCoreTag ctor (skip reset) ~1.02×
- H18 wTr via copyStitchCore+clear
- H19 `compareSeqToGenome` 8-byte blocks
- H20 recursive Transcript fork pool ~**1.015×** (best near-miss)
- H21 extend pointer specialize / budget
- H22 `-march=native` (LTO incompatible with htslib link here)
- H23 fixture PGO (slower)
- H24 POD layout + full-prefix memcpy (slower than partial copy)
- H25 `-funroll-loops` / large inline limit

## Final measurement (candidate-wave3 == best)

- **Baseline median:** 2.22 s  
- **Candidate median:** 1.39 s  
- **Median speedup:** **1.597×** (9 paired rounds)  
- **Gap to 2×:** ≈1.252× more required  
- **≥2.0?** No → no N=30, no lower_95, **not QUALIFIED**

## Remaining gap & ceilings

Closing 2× without forbidden shortcuts requires removing a large share of **necessary** stitch/extend/SA work or changing the include/exclude search tree in a score-preserving way. Micro-optimizations that preserve exact alignment semantics are largely saturated after Wave2 copy elimination. Theoretical Amdahl half-bundle ceilings are not evidence of achievability.

## Forbidden shortcuts explicitly not used
- Threads / OpenMP bump / `bgzf_mt`
- Weaker validation / CRC skip / oracle edits
- Cross-run result cache

## Artifacts
- Best binary: `best_patch/STAR` (sha256 in `best_patch/binary.sha256`)
- Best patch: `best_patch/combined.patch`
- Reproduce: `REPRODUCE.md`
- Hypotheses ledger: `hypotheses.jsonl`
- Wave3 writeup: `evidence/WAVE3_SUMMARY.md`
- Paired compare: `evidence/COMPARE.md` / `evidence/COMPARE.json`

## Next hypotheses (not claimed as wins)
1. POD-only `StitchTr` recursion type (no vector/set in hot forks).
2. Transactional `stitchAlignToTranscript` + mutate/undo with small deltas.
3. Proven score-equivalent stitch search pruning.
4. SA compare only after further stitch/copy reduction.

## Terminal status

**NOT_YET** — best speedup ≈1.60×; 2× not established; do not claim QUALIFIED.
