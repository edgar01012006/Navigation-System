## Data Setup

To keep the repository clean and lightweight, the raw map data is not included in source control.

Please follow these steps to obtain the dataset:

1. Open your browser and navigate to [OpenStreetMap](https://www.openstreetmap.org/).
2. Use the search bar on the left to search for **Yerevan**.
3. Click the **Export** button at the top of the page.
4. In the export sidebar, click the blue **Overpass API** link (this will automatically download the bounding box extract).
5. Locate the downloaded file (typically named `map`), rename it to **`yerevan.osm`**, and move it into the `data/` directory of this project.

The folder structure should look like this:

```text
Navigation-System/
├── data/
│   └── yerevan.osm
...
```

---

## Building and Running

### Prerequisites

- A C++ compiler supporting C++20
- `make`

### Compilation

Open your terminal in the project root directory and run:

```bash
make
```

### Execution

Run the compiled binary from the project root:

```bash
./bin/YerevanNav
```

### Clean Build Files

To wipe temporary build artifacts and object files, use:

```bash
make clean
```