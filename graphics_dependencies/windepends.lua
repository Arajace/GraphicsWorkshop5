configuration "windows"
    if _ACTION ~= "clean" then
          --os.execute("windll.bat")
    end

    if _ACTION == "clean" then
        os.execute("windll.bat clean")
    end
