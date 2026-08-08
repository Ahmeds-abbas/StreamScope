# ADR 001: Segment Control Boundary

- Status: Accepted
- Date: 2026-08-08

## Context

StreamScope plays HLS streams through GStreamer. HLS playback involves discovering
variant playlists, selecting a rendition, downloading media segments, buffering,
retrying failed requests, and switching renditions as network conditions change.

Implementing those responsibilities directly in StreamScope would duplicate the
behavior already provided by GStreamer's HLS and playback elements. It would also
couple the application to transport and demuxing details before the monitoring
requirements are fully understood.

StreamScope still needs visibility into playback so that it can report useful
information such as state changes, buffering, errors, and quality changes.

## Decision

GStreamer owns the segment-control boundary.

StreamScope will provide the top-level stream URI and control the player lifecycle,
including play, pause, seek, and stop. GStreamer will own:

- Loading and parsing HLS manifests.
- Selecting HLS variants.
- Requesting, buffering, and retrying media segments.
- Demuxing, decoding, and synchronizing media.
- Adaptive bitrate decisions.

StreamScope may observe GStreamer messages, properties, signals, and debugging
hooks to collect playback telemetry. Observation must not require StreamScope to
download segments itself or make per-segment scheduling decisions.

If a future feature requires direct segment control, that requirement must be
documented in a new ADR before moving responsibility across this boundary.

## Consequences

### Positive

- The initial player remains small and relies on established GStreamer behavior.
- StreamScope can focus on playback control, diagnostics, and presentation.
- Protocol details remain isolated from the application layer.
- Improvements and fixes in GStreamer can benefit StreamScope automatically.

### Negative

- Available telemetry is limited to information exposed by GStreamer.
- Some segment-level behavior may be difficult to customize.
- GStreamer plugin behavior and version differences can affect playback.
- Precise segment instrumentation may require probes or custom GStreamer elements.

## Revisit When

Reconsider this decision if StreamScope needs capabilities that GStreamer cannot
expose or support, such as deterministic segment selection, custom download
scheduling, segment rewriting, or transport experiments requiring control before
data enters the GStreamer pipeline.
