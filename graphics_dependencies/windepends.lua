configuration {"WINDOWS"}
    if _ACTION ~= "clean" then
          os.execute("windll.bat")
    elseif _ACTION == "clean" then
        os.execute("windll.bat clean")
    end
