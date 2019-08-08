_G["__RENDERER_ELEMENTS__"] = {}    -- global table containing the renderer elements
_G["__RENDERER_ELEMENTS__"]["__CAMERA__"] = {}
_G["__RENDERER_ELEMENTS__"]["__PIXEL_SAMPLER__"] = {}
_G["__RENDERER_ELEMENTS__"]["__IMAGE_BUFFER__"] = {}
_G["__RENDERER_ELEMENTS__"]["__GLOBAL_SETTINGS__"] = {}
_G["__RENDERER_ELEMENTS__"]["__OBJECTS__"] = {}
_G["__RENDERER_ELEMENTS__"]["__OBJECT_COUNT__"] = 0
_G["__RENDERER_ELEMENTS__"]["__BSDFS__"] = {}
_G["__RENDERER_ELEMENTS__"]["__BSDF_COUNT__"] = 0
_G["__RENDERER_ELEMENTS__"]["__LAYERED_BSDFS__"] = {}
_G["__RENDERER_ELEMENTS__"]["__LAYERED_BSDF_COUNT__"] = 0
_G["__RENDERER_ELEMENTS__"]["__EMISSIONS__"] = {}
_G["__RENDERER_ELEMENTS__"]["__EMISSION_COUNT__"] = 0
_G["__RENDERER_ELEMENTS__"]["__MATERIALS__"] = {}
_G["__RENDERER_ELEMENTS__"]["__MATERIAL_COUNT__"] = 0
_G["__RENDERER_ELEMENTS__"]["__INTEGRATOR__"] = {}

function PrintBSDF(bsdf)
    print("    type: " .. bsdf.type)
    if (bsdf.type == "lambertian") then
        print("    kd: [" .. bsdf.kd[1] .. ", "
                          .. bsdf.kd[2] .. ", "
                          .. bsdf.kd[3] .. "]")
    elseif (bsdf.type == "smooth_conductor") then
        print("    reflectance_at_normal_incidence: [" .. bsdf.reflectance_at_normal_incidence[1] .. ", "
                                                       .. bsdf.reflectance_at_normal_incidence[2] .. ", "
                                                       .. bsdf.reflectance_at_normal_incidence[3] .. "]")
    elseif (bsdf.type == "smooth_dielectric") then
        print("    ior: " .. bsdf.ior)
    elseif (bsdf.type == "cook_torrance") then
        print("    roughness: " .. bsdf.roughness)
        print("    reflectance_at_normal_incidence: [" .. bsdf.reflectance_at_normal_incidence[1] .. ", "
                                                       .. bsdf.reflectance_at_normal_incidence[2] .. ", "
                                                       .. bsdf.reflectance_at_normal_incidence[3] .. "]")
    end
    print("    sampling: " .. bsdf.surface_sampler)
end

function PrintBSDFList()
    bsdf_count = _G["__RENDERER_ELEMENTS__"]["__BSDF_COUNT__"]

    for i = 1, bsdf_count, 1 do
        print("BSDF id: " .. i)
        bsdf = _G["__RENDERER_ELEMENTS__"]["__BSDFS__"][i]
        if (bsdf.type == "layered") then
            print ("  number of layers: " .. bsdf.num_layers)

            local ordered_keys = {}

            for k in pairs(bsdf) do
                if (type(k) == "number") then
                   table.insert(ordered_keys, k)
                end
            end

            table.sort(ordered_keys)

            for i = 1, #ordered_keys do
                local k = ordered_keys[i]
                local bsdf_id = bsdf[ordered_keys[i] ]
                print("  >> layer " .. k .. " ( BSDF id " .. bsdf_id .. ")")
                local bsdf = _G["__RENDERER_ELEMENTS__"]["__BSDFS__"][bsdf_id]
                PrintBSDF(bsdf)
            end
        else
            PrintBSDF(_G["__RENDERER_ELEMENTS__"]["__BSDFS__"][i])
        end
    end
end

function PrintLayeredBSDFList()
    layered_bsdf_count = _G["__RENDERER_ELEMENTS__"]["__LAYERED_BSDF_COUNT__"]

    for i = 1, layered_bsdf_count, 1 do
        print("Layered BSDF id: " .. i)

        layered_bsdf = _G["__RENDERER_ELEMENTS__"]["__LAYERED_BSDFS__"][i]
        print ("  number of layers: " .. layered_bsdf.num_layers)

        local ordered_keys = {}

        for k in pairs(layered_bsdf) do
            if (type(k) == "number") then
               table.insert(ordered_keys, k)
            end

            --print ("-----> " .. k .. " : " .. v)
        end

        table.sort(ordered_keys)


        for i = 1, #ordered_keys do
            local k = ordered_keys[i]
            local bsdf_id = layered_bsdf[ordered_keys[i] ]
            print("  >> layer " .. k .. " ( BSDF id " .. bsdf_id .. ")")
            local bsdf = _G["__RENDERER_ELEMENTS__"]["__BSDFS__"][bsdf_id]
            PrintBSDF(bsdf)
        end
        --]]
    end
end

function PrintEmissionList()
    emission_count = _G["__RENDERER_ELEMENTS__"]["__EMISSION_COUNT__"]

    for i = 1, emission_count, 1 do
        print("Emission id: " .. i)
        emission = _G["__RENDERER_ELEMENTS__"]["__EMISSIONS__"][i]
        print("  >> emission: " .. emission[1] .. ", " .. emission[2] .. ", " .. emission[3])
    end
end

function PrintMaterialList()
    material_count = _G["__RENDERER_ELEMENTS__"]["__MATERIAL_COUNT__"]

    for i = 1, material_count, 1 do
        print("Material id: " .. i)
        material = _G["__RENDERER_ELEMENTS__"]["__MATERIALS__"][i]
        print("  >> bsdf: " .. material.bsdf .. " / " .. _G["__RENDERER_ELEMENTS__"]["__BSDFS__"][material.bsdf].type)
        print("  >> emission: " .. material.emission)
    end
end

-- The functions below are just for DEBUG purposes
--[[

function PrintEmission(v)
    emission = _G["__RENDERER_ELEMENTS__"]["__EMISSIONS__"][v.emission]
    print("            emission: " .. emission[1] .. ", " .. emission[2] .. ", " .. emission[3])
end

function PrintMaterial(v)
    bsdf = _G["__RENDERER_ELEMENTS__"]["__BSDFS__"][v.bsdf]

    print("        Material:")
    print("            id: " .. v.bsdf)

    if (bsdf.type == "layered") then
        local ordered_keys = {}

        for k in pairs(bsdf) do
            if (type(k) == "number") then
               table.insert(ordered_keys, k)
            end
        end

        table.sort(ordered_keys)

        for i = 1, #ordered_keys do
            local k = ordered_keys[i]
            local bsdf_id = bsdf[ordered_keys[i] ]
            print("            >>> layer " .. k)
            local bsdf = _G["__RENDERER_ELEMENTS__"]["__BSDFS__"][bsdf_id]
            PrintBSDF(bsdf)
        end
    else
        print("            number of layers: 1")
        PrintBSDF(bsdf)
    end

    PrintEmission(v)
end

function TellMe()
    ---------------------------------------------------------------------------
    print(">>> Camera")
    print("    type: " .. _G["__RENDERER_ELEMENTS__"]["__CAMERA__"].type)
    print("    position: ["
          .. _G["__RENDERER_ELEMENTS__"]["__CAMERA__"].position[1] .. ", "
          .. _G["__RENDERER_ELEMENTS__"]["__CAMERA__"].position[2] .. ", "
          .. _G["__RENDERER_ELEMENTS__"]["__CAMERA__"].position[3] .. "]")
    print("    up: " .. _G["__RENDERER_ELEMENTS__"]["__CAMERA__"].up[1] .. ", "
                     .. _G["__RENDERER_ELEMENTS__"]["__CAMERA__"].up[2] .. ", "
                     .. _G["__RENDERER_ELEMENTS__"]["__CAMERA__"].up[3] .. "]")
    print("    look_at: " .. _G["__RENDERER_ELEMENTS__"]["__CAMERA__"].look_at[1] .. ", "
                          .. _G["__RENDERER_ELEMENTS__"]["__CAMERA__"].look_at[2] .. ", "
                          .. _G["__RENDERER_ELEMENTS__"]["__CAMERA__"].look_at[3] .. "]")
    print("    aspect: " .. _G["__RENDERER_ELEMENTS__"]["__CAMERA__"].aspect)

    if (_G["__RENDERER_ELEMENTS__"]["__CAMERA__"].type == "pinhole") then
        print("    fov: " .. _G["__RENDERER_ELEMENTS__"]["__CAMERA__"].fov)
    elseif  (_G["__RENDERER_ELEMENTS__"]["__CAMERA__"].type == "orthographic") then
        print("    minx: " .. _G["__RENDERER_ELEMENTS__"]["__CAMERA__"].minx)
        print("    maxx: " .. _G["__RENDERER_ELEMENTS__"]["__CAMERA__"].maxx)
        print("    miny: " .. _G["__RENDERER_ELEMENTS__"]["__CAMERA__"].miny)
        print("    maxy: " .. _G["__RENDERER_ELEMENTS__"]["__CAMERA__"].maxy)
    end

    ---------------------------------------------------------------------------
    print()
    print(">>> Sampler")
    print("    hres: " .. _G["__RENDERER_ELEMENTS__"]["__SAMPLER__"].type)
    print("    vres: " .. _G["__RENDERER_ELEMENTS__"]["__SAMPLER__"].spp)

    ---------------------------------------------------------------------------
    print()
    print(">>> Buffer")
    print("    hres: " .. _G["__RENDERER_ELEMENTS__"]["__BUFFER__"].hres)
    print("    vres: " .. _G["__RENDERER_ELEMENTS__"]["__BUFFER__"].vres)

    ---------------------------------------------------------------------------
    print()
    print(">>> Global Settings")
    print("    background_color: [" .. _G["__RENDERER_ELEMENTS__"]["__GLOBAL_SETTINGS__"].background_color[1] .. ", "
                                    .. _G["__RENDERER_ELEMENTS__"]["__GLOBAL_SETTINGS__"].background_color[2] .. ", "
                                    .. _G["__RENDERER_ELEMENTS__"]["__GLOBAL_SETTINGS__"].background_color[3] .. "]")
    print("    path_termination: " .. _G["__RENDERER_ELEMENTS__"]["__GLOBAL_SETTINGS__"].path_termination)
    print("    path_length: " .. _G["__RENDERER_ELEMENTS__"]["__GLOBAL_SETTINGS__"].path_length)
    print("    image_filename: " .. _G["__RENDERER_ELEMENTS__"]["__GLOBAL_SETTINGS__"].image_filename)
    print("    acceleration_structure: " .. _G["__RENDERER_ELEMENTS__"]["__GLOBAL_SETTINGS__"].acceleration_structure)

    ---------------------------------------------------------------------------
    print()
    print(">>> Objects")

    print("    >>> object count: " .. _G["__RENDERER_ELEMENTS__"]["__OBJECT_COUNT__"])

    for n,v in pairs(_G["__RENDERER_ELEMENTS__"]["__OBJECTS__"]) do
        print ("    >>> " .. n .. ":  " .. v.type)
        PrintMaterial(v)
    end
end

function PrintBSDFList()
    bsdf_count = _G["__RENDERER_ELEMENTS__"]["__BSDF_COUNT__"]

    for i = 1, bsdf_count, 1 do
        print("BSDF id: " .. i)
        bsdf = _G["__RENDERER_ELEMENTS__"]["__BSDFS__"][i]
        print ("    number of layers: " .. bsdf.num_layers)

        if (bsdf.type == "layered") then
            local ordered_keys = {}

            for k in pairs(bsdf) do
                if (type(k) == "number") then
                   table.insert(ordered_keys, k)
                end
            end

            table.sort(ordered_keys)

            for i = 1, #ordered_keys do
                local k = ordered_keys[i]
                local bsdf_id = bsdf[ordered_keys[i] ]
                print("  >> layer " .. k .. " ( BSDF id " .. bsdf_id .. ")")
                local bsdf = _G["__RENDERER_ELEMENTS__"]["__BSDFS__"][bsdf_id]
                PrintBSDF(bsdf)
            end
        else
            PrintBSDF(_G["__RENDERER_ELEMENTS__"]["__BSDFS__"][i])
        end
    end
end
--]]