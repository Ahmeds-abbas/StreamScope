import json
import sys
from collections import Counter


def load_events(path):
    events = []

    with open(path, "r") as file:
        for line in file:
            line = line.strip()

            if line:
                events.append(json.loads(line))

    return events


def summarize(events):
    downloads = [
        event
        for event in events
        if event.get("event") == "segment_download_completed"
    ]

    selections = [
        event
        for event in events
        if event.get("event") == "representation_selected"
    ]

    ended_events = [
        event
        for event in events
        if event.get("event") == "playback_state_changed"
        and event.get("state") == "Ended"
    ]

    quality_counts = Counter(
        event["height"]
        for event in selections
    )

    average_throughput = (
        sum(event["throughput_mbps"] for event in downloads)
        / len(downloads)
        if downloads
        else 0.0
    )

    return {
        "segments": len(downloads),
        "last_download_ms": downloads[-1]["timestamp_ms"] if downloads else 0,
        "ended_ms": ended_events[-1]["timestamp_ms"] if ended_events else 0,
        "average_throughput_mbps": average_throughput,
        "qualities": dict(quality_counts),
    }


def main():
    if len(sys.argv) != 3:
        print("Usage: python3 compare_runs.py <abr.jsonl> <fixed.jsonl>")
        return 1

    abr = summarize(load_events(sys.argv[1]))
    fixed = summarize(load_events(sys.argv[2]))

    print("\nStreamScope Tier 1 Comparison")
    print("-----------------------------")

    print(f"ABR segments:          {abr['segments']}")
    print(f"Fixed segments:        {fixed['segments']}")

    print(
        f"ABR final download:    "
        f"{abr['last_download_ms'] / 1000:.2f} s"
    )

    print(
        f"Fixed final download:  "
        f"{fixed['last_download_ms'] / 1000:.2f} s"
    )

    print(
        f"ABR playback end:      "
        f"{abr['ended_ms'] / 1000:.2f} s"
    )

    print(
        f"Fixed playback end:    "
        f"{fixed['ended_ms'] / 1000:.2f} s"
    )

    print(
        f"ABR avg throughput:    "
        f"{abr['average_throughput_mbps']:.2f} Mbps"
    )

    print(
        f"Fixed avg throughput:  "
        f"{fixed['average_throughput_mbps']:.2f} Mbps"
    )

    print(f"ABR quality choices:   {abr['qualities']}")
    print(f"Fixed quality choices: {fixed['qualities']}")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
