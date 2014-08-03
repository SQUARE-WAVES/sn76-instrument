local concat = function(tab,val)
	local ret = {}
	for i,v in ipairs(tab) do
		table.insert(ret,v)
	end

	table.insert(ret,val)

	return ret
end

local function create_observer(tab,observer,path)

	local mt = {}
	local makepath = function(key)
		return {key}
	end

	if(type(path) == 'table') then
		makepath = function(key)
			return concat(path,key)
		end
	end

	mt.__newindex = function(ref,key,val)
		tab[key] = val
		observer(makepath(key),val)
	end

	mt.__index = function(ref,key)
		local val = tab[key]
		if(type(val) == 'table') then
			return create_observer(val,observer,makepath(key))
		else
			return val
		end
	end

	local ref = {}
	setmetatable(ref,mt)

	return ref
end

return create_observer