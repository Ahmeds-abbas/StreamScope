import json
import subprocess
import sys
import time
from datetime import datetime
from pathlib import Path


def load_scenario(path):
    with open(path, "r") as file:
        return json.load(file)


def apply_bandwidth(bandwidth_mbps):
    command = [
        "sudo",
        "tc",
        "qdisc",
        "replace",
        "dev",
        "lo",
        "root",
        "tbf",
        "rate",
        f"{bandwidth_mbps}mbit",
        "burst",
        "128kb",
        "latency",
        "400ms",
    ]

    subprocess.run(command, check=True)


def clear_bandwidth():
    subprocess.run(
        [
            "sudo",
            "tc",
            "qdisc",
            "del",
            "dev",
            "lo",
            "root",
        ],
        check=False,
    )


def start_player(repo_root, telemetry_path):
    player_path = (
        repo_root
        / "build"
        / "player"
        / "streamscope_player"
    )

    command = [
        str(player_path),
        "http://127.0.0.1:8000/master.m3u8",
        "--mode",
        "abr",
        "--telemetry",
        str(telemetry_path),
    ]

    return subprocess.Popen(
        command,
        cwd=repo_root,
    )


def main():
    if len(sys.argv) != 2:
        print(
            "Usage: python3 scenario_runner.py <scenario.json>"
        )
        return 1

    scenario = load_scenario(sys.argv[1])

    repo_root = Path(__file__).resolve().parents[1]

    telemetry_directory = repo_root / "telemetry" / "runs"
    telemetry_directory.mkdir(parents=True, exist_ok=True)

    timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
    telemetry_path = (
        telemetry_directory
        / f"{scenario['name']}_{timestamp}.jsonl"
    )

    print(f"Scenario: {scenario['name']}")

    # Ask for sudo authentication before timing begins.
    subprocess.run(["sudo", "-v"], check=True)

    player = None

    try:
        player = start_player(repo_root, telemetry_path)

        for index, step in enumerate(scenario["steps"]):
            bandwidth = step["bandwidth_mbps"]
            duration = step["duration_seconds"]

            print(
                f"Step {index}: "
                f"{bandwidth} Mbps "
                f"for {duration} seconds"
            )

            apply_bandwidth(bandwidth)
            time.sleep(duration)

        print("Waiting for StreamScope to finish...")

        return_code = player.wait()

        print(
            f"StreamScope exited with code "
            f"{return_code}"
        )

        return return_code

    finally:
        print("Restoring normal network...")
        clear_bandwidth()

        if player is not None and player.poll() is None:
            print("Stopping StreamScope...")
            player.terminate()
            player.wait()


if __name__ == "__main__":
    raise SystemExit(main())
