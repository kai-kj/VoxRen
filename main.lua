function Main()
	outputProperties(800, 600)

	local size = 10

	local width = size
	local height = size
	local depth = size

	bgProperties(0.5, 0.5, 1, 0.5)

	local white = lambertMaterial(0.8, 0.8, 0.8)
	local red = lambertMaterial(1, 0, 0)
	local green = lambertMaterial(0, 1, 0)
	local blue = lambertMaterial(0, 0, 1)
	local light = lightSource(1, 1, 0.5, 2)
	local metal = metalMaterial(0.5, 0.5, 0.5, 1, 0, 0.5);

	-- side walls
	for y = 0, height - 1, 1 do
		for z = 0, depth - 1, 1 do
			addVox(0, y, z, red)
			addVox(width - 1, y, z, green)
		end
	end

	-- back wall
	for x = 0, width - 1, 1 do
		for y = 0, height - 1, 1 do
			addVox(x, y, depth - 1, blue)
		end
	end

	-- celling / floor
	for x = 0, width - 1, 1 do
		for z = 0, depth - 1, 1 do
			addVox(x, 0, z, white)
			addVox(x, height - 1, z, white)
		end
	end

	-- test chunk add/removal
	addVox(2 * width, 0, 0, white)
	remVox(2 * width, 0, 0)

	-- objects
	addVox(width * 0.7, height - 1, depth * 0.5, light)
	addVox(width * 0.7, height - 2, depth * 0.5, light)

	addVox(width * 0.3, height - 1, depth * 0.3, metal)
	addVox(width * 0.3, height - 2, depth * 0.3, metal)

	addVox(width * 0.5, height - 2, depth * 0.5, red)

	cameraProperties(1, 1, 0.001, 1000)
	cameraPos(5.5, -2, -10, 0, -math.pi / 8)

end