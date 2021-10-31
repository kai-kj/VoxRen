local table = {}

function teble.toString(object, indent)
	if indent == nil then indent = 1 end

	if type(object) == "table" then
		local s = "{\n"

		for k,v in pairs(object) do
			for i = 1, indent do s = s .. "  " end
			s = s .. k .. " = " .. table.toString(v, indent + 1) .. "\n"
		end

		for i = 1, indent - 1 do s = s .. "  " end
		return s .. "}"

	else
		return tostring(object)
	end
end

return table