# CISM

CISM (Customizable Interactive Script Manager) is a simple tool for managing shell scripts for your project.

## Requirements

- A C++20 compiler
- [Nlohmann's JSON library](https://github.com/nlohmann/json)
- [CPack](https://cmake.org/cmake/help/latest/module/CPack.html)
- [LCOV](https://github.com/linux-test-project/lcov)

## Installation

1. Clone or download this repository to your local machine.
2. Navigate to the cloned directory using a terminal.
3. Run the following command to install the required dependencies:

    ```bash
    mkdir build && cd build
    cmake ..
    make install
    ```

## Configuration

Create a file named `.cism.json` in the root directory of your project. The file should contain the following JSON 

```json
{
    ...
    "path": "src/scripts",
    "scripts": {...}
    ...
}
```

Cism allows you to setup scripts adapting to your needs:

```json
{
    "path": "src/scripts",
    "scripts": {
        "script-in-src/scripts": "script-name.sh",
        "relative-path": "path/to/script-name.sh",
        "absolute-path": "/path/to/script-name.sh",
        "script-with-args": "script-name.sh arg1 arg2 arg3",
        "system-command": "ls -l",
        "system-command:absolute-path": "/bin/ls -l"
    }
}
```

## Usage

### Calling a script

To call a script, run the following command:

```bash
cism <script-name>
```

Calling script passing arguments:

```bash
cism <script-name> <arg1> <arg2> ...
```

## Contributing

Contributions are welcome! If you find a bug or have an idea for a new feature, please open an issue or create a pull request.

## License

CISM is licensed under the MIT license. See the `LICENSE` file for more information.
