function Main()
	r.set_output_properties(800, 600)

	local size = 10

	local width = size
	local height = size
	local depth = size

	r.set_background_properties(0.5, 0.5, 1, 0.1)

	local white = r.create_lambertian_material(0.8, 0.8, 0.8)
	local red = r.create_lambertian_material(1, 0, 0)
	local green = r.create_lambertian_material(0, 1, 0)
	local blue = r.create_lambertian_material(0, 0, 1)
	local light = r.create_light_source_material(1, 1, 0.5, 2)

	-- side walls
	for y = 0, height - 1, 1 do
		for z = 0, depth - 1, 1 do
			r.add_voxel(0, y, z, red)
			r.add_voxel(width - 1, y, z, green)
		end
	end

	-- back wall
	for x = 0, width - 1, 1 do
		for y = 0, height - 1, 1 do
			r.add_voxel(x, y, depth - 1, blue)
		end
	end

	-- celling / floor
	for x = 0, width - 1, 1 do
		for z = 0, depth - 1, 1 do
			r.add_voxel(x, 0, z, white)
			r.add_voxel(x, height - 1, z, white)
		end
	end

	-- test chunk add/removal
	r.add_voxel(2 * width, 0, 0, white)
	r.remove_voxel(2 * width, 0, 0)

	-- objects
	r.add_voxel(width * 0.7, height - 1, depth * 0.5, light)
	r.add_voxel(width * 0.7, height - 2, depth * 0.5, light)

	r.add_voxel(width * 0.3, height - 1, depth * 0.3, white)
	r.add_voxel(width * 0.3, height - 2, depth * 0.3, white)

	r.set_camera_properties(1, 1, 0.001, 1000)
	r.set_camera_pos(5.5, -2, -10, 0, -math.pi / 8)

end