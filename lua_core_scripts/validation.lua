function CheckVector3(v)
    if (#v ~= 3) then
        return false
    end

    return (type(v[1]) == "number") and
           (type(v[2]) == "number") and
           (type(v[3]) == "number")
            
end

function CheckVector3Field(obj, field_str, default)
    if (obj[field_str] == nil) then
        obj[field_str] = default
        return true
    end
        
    return CheckVector3(obj[field_str])
end

function CheckNumberField(obj, field_str, default)
    if (obj[field_str] == nil) then
        if (default == nil) then
            return false
        end

        obj[field_str] = default
        return true
    end
    
    return type(obj[field_str]) == "number"
end

function CheckStringField(obj, field_str, default, values)
    if (obj[field_str] == nil) then
        if (default == nil) then
            return false
        end
        
        obj[field_str] = default
        return true
    end

    if (not (values == nil)) then
        result = false

        for value_count = 1, #values do
            if (obj[field_str] == values[value_count]) then
                result = true
            end
        end    
        
        return result
    end
        
    return type(obj[field_str]) == "string"
end
