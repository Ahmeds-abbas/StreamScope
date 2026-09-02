import json
import subprocess
import sys
import time


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


def main():
    if len(sys.argv) != 2:
        print(
            "Usage: python3 scenario_runner.py <scenario.json>"
        )
        return 1

    scenario = load_scenario(sys.argv[1])

    print(f"Scenario: {scenario['name']}")

    try:
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

    finally:
        print("Restoring normal network...")
        clear_bandwidth()

    print("Scenario completed.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())