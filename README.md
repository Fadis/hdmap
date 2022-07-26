# hdmap

High Dimensional Map for C++.

hdmap is a sparse compact spatial data structure that supports finding, updating and erasing voxels in specified axis-aligned bounding box region.

hdmap is implemented as header-only library, so it is easy to embed anywhere.

## Requirements

C++ compiler that supports C++20 or higher.

[Boost](https://www.boost.org/) : Container, Integer and Unit Test Framework (for tests) are needed.

(Optional) [Nlohmann JSON](https://github.com/nlohmann/json) : Needed to serialize the data structure into JSON.

