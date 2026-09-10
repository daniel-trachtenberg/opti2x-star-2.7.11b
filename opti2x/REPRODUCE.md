# Reproduce best STAR Opti2x candidate (NOT_YET — not 2×)

## Contract
- `optimization.yaml` / `star-2.7.11b-serial-align-fixture-v1`
- `--runThreadN 1`, `genomeLoad NoSharedMemory`, BAM Unsorted
- NLWP must remain 1; **do not** enable `bgzf_mt` or raise threads
- Oracle: sorted SAM body sha256 `20739c12f887bbf5b5eba10c29d4546ac8f4fb3d9996079ccc377162abbce483`
- Development fixture only (`benchmarks/development.yaml`); do not edit oracle

## Build
```bash
cd /workspace/opti2x/runs/star-2.7.11b/experiments/candidate-wave3/worktree/source
make clean && make STAR
# or use prebuilt: experiments/candidate-wave3/STAR
# patch vs baseline tree: experiments/candidate-wave3/combined.patch
#   (also copied to deliverable-NOT_YET/best_patch/combined.patch)
```

## Timing + oracle (serialize)
```bash
ROOT=/workspace/opti2x/runs/star-2.7.11b
$ROOT/experiments/run_oracle_timing.sh \
  $ROOT/experiments/candidate-wave3/STAR \
  /tmp/star-cand-w3 1 5
# NLWP spot-check:
$ROOT/experiments/nlwp_probe.sh $ROOT/experiments/candidate-wave3/STAR /tmp/star-nlwp
```

Paired baseline compare: run baseline/STAR then candidate each round with warm page cache; see `evidence/COMPARE.md`.

## Expected
- Oracle PASS
- NLWP=1
- Median speedup vs baseline ≈ **1.59–1.60×** (not ≥2.0)
- Status: **NOT_YET** — do not mark QUALIFIED
