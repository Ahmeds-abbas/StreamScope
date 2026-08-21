## Tier 1 Results

StreamScope was evaluated under a controlled 3 Mbps loopback bandwidth limit using Linux traffic control.

| Policy | Final Segment Download | Playback End | Quality Selection |
|---|---:|---:|---|
| Throughput ABR | 18.16 s | 63.69 s | 14×360p, 1×720p |
| Fixed 1080p | 87.12 s | 87.45 s | 15×1080p |

Under the same constrained network condition, the throughput-based ABR policy completed playback approximately 27% faster than fixed 1080p by adapting representation quality to measured throughput.

The Tier 1 ABR policy uses an 80% throughput safety factor and starts at the lowest representation when no prior network measurement is available.

### Reproducing the comparison

```bash
python3 experiments/compare_runs.py \
    telemetry/abr_3mbps.jsonl \
    telemetry/fixed_1080p_3mbps.jsonl
```
