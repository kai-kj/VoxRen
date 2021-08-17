# ToDo list

## Core

* [ ] Rendering
  * [x] Better logging
    * [x] Print newline automatically
    * [x] Use `msg()` to show detailed info about the status of the program
    * [x] Implement `dbg()`, `err()`, and `panic()`
    * [x] Better error checking
  * [ ] Transparent materials
* [ ] Basic GUI
  * [x] Render to SDL window
  * [x] Keyboard movement
  * [ ] Integrate command system to GUI
  * [x] Mouse right drag to change view angle
  * [ ] Place voxels anywhere on the screen with mouse
* [ ] Command system (LUA)
  * [ ] Get lua working
  * [ ] Basic C commands
  * [ ] Basic lua commands
* [ ] Read write scene to file
* [ ] Other
  * [ ] Windows support

## Optimization

* [ ] Rendering
  * [x] Chunks Voxel storage format in C and openCL:

    ```
    struct Chunk {
    	cl_int x;
    	cl_int y;
    	cl_uint voxelIdx;
    	cl_uint voxelCount;
    }
    
    
    ```

    New `add_voxel()` function:

    ```
    
    add_voxel() {
    	voxel = create_voxel()
    
    	chunk = get_chunk_for_voxel()
    
    	if not chunk_exists(chunk) {
    		add_chunk(chunk)
    	}
    
    	add_voxel_to_chunk(chunk, voxel)
    }
    
    add_voxel_to_chunk() {
    	
    }
    
    
    ```
  * [ ] Use unsigned values where possible
  * [ ] Use pointers for almost everything
* [ ] GUI
  * [x] Only update renderer surface when it changes