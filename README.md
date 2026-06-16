# Wave Function Collapse Implementation

## Overview

This Unreal Engine component implements a **Wave Function Collapse (WFC)** algorithm for procedural generation in both 2D and 3D spaces. The system uses constraint propagation to generate coherent patterns by analyzing spatial relationships and applying rules based on adjacency.

## Features

- **3D Generation Mode**: Creates volumetric structures using a three-dimensional grid
- **2D Generation Mode**: Produces flat patterns on surfaces
- **Constraint-Based Propagation**: Automatically eliminates invalid combinations using neighbor constraints
- **Configurable Resolution**: Adjustable grid density via `PointsNum` property
- **Modular Actor System**: Uses actor classes as "tiles" for generation

## How It Works

### Generation Process

1. **Grid Setup**: Creates a grid of sample points within the owner actor's bounding box
2. **Surface Detection**: Uses line traces to identify valid placement positions
3. **Constraint Analysis**: For each position, determines which actors are valid based on:
   - Surface normal direction (3D mode)
   - Adjacent positions (both modes)
   - Pre-defined rules in the `ToSpawnArray`
4. **Wave Collapse**: Randomly selects valid actors and propagates constraints to neighboring cells

### Key Methods

- `ThreeDimGeneration()`: Sets up a 3D grid and performs line traces outward from the center
- `TwoDimGeneration()`: Creates a 2D grid with downward line traces for surface detection
- `SpawnActorsThreeDim()`: Spawns actors in 3D space with normal-aligned rotation
- `SpawnActorsTwoDim()`: Spawns actors on a 2D surface with Z-alignment
- `CalculateOffsetDirections()`: Core constraint propagation logic - disallows adjacent combinations

## Usage

### Setup

1. Add this component to any Actor in your Unreal Engine project
2. Populate the `ToSpawnArray` with actor classes to be used as tiles
3. Set `PointsNum` to control grid resolution (higher = more detail)

### Properties

- `bTwoDGeneration`: Toggle between 2D and 3D generation modes
- `PointsNum`: Number of points along each axis (grid resolution)
- `PointToSpawn`: Actor used as temporary grid markers (visual debugging)
- `ToSpawnArray`: Array of actor classes available for generation
- `Gap`: Automatically calculated spacing between grid points

### Constraint Rules

The system applies the following constraints:
- **3D Mode**: Checks forward, backward, right, and left neighbors
- **2D Mode**: Checks forward and right neighbors only
- **Custom Rules**: The `CalculateOffsetDirections` method applies specific rules based on actor indices (e.g., index 0 and 3 have special handling)

## Limitations & Considerations

- **Performance**: High `PointsNum` values can impact performance (grid size grows exponentially in 3D)
- **Actor Requirements**: All actors in `ToSpawnArray` must be spawnable via `SpawnActor`
- **Collision**: Line traces rely on proper collision channel configuration
- **Memory**: The system stores boolean arrays for each grid position (size = grid cells × actor count)

## Future Improvements

- Add support for weighted random selection
- Implement multi-stage generation
- Add support for custom constraint rules
- Optimize memory usage for large grids
- Add visual debug overlay

## Dependencies

- Unreal Engine (Core, RenderCore, Kismet libraries)
- Requires `DrawDebugHelpers` for debugging (optional)