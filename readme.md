## Opengl example 

### cmake vscode setting
- c_cpp_properties.json

    <pre><code>
    {
        "configurations": [
            {
                "name": "Mac",
                "includePath": [
                    "${workspaceFolder}/**",
                    "${workspaceFolder}/build/install/include/**",
                    "${workspaceFolder}/src/**"
                ],
                "defines": [],
                "macFrameworkPath": [
                    "/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/System/Library/Frameworks"
                ],
                "compilerPath": "/usr/bin/clang",
                "cStandard": "c17",
                "cppStandard": "c++17",
                "intelliSenseMode": "macos-clang-x64",
                "configurationProvider": "vscode.cmake-tools"
            }
        ],
        "version": 4
    }
    </code></pre>
