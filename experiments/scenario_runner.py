import json
import sys


def load_scenario(path):
    with open(path, "r") as file:
        return json.load(file)


def main():
    if len(sys.argv) != 2:
        print("Usage: python3 scenario_runner.py <scenario.json>")
        return 1

    scenario_path = sys.argv[1]

    scenario = load_scenario(scenario_path)

    print(f"Scenario: {scenario['name']}")

    for index, step in enumerate(scenario["steps"]):
        print(
            f"Step {index}: "
            f"{step['bandwidth_mbps']} Mbps "
            f"for {step['duration_seconds']} seconds"
        )

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
