# QtOpenAI

**QtOpenAI** is a C++/Qt library that simplifies usage of various OpenAI APIs (Chat, Embeddings, Image Generation, etc.). It leverages Qt’s networking and signal/slot mechanisms to perform asynchronous requests to OpenAI endpoints.

---

## Features

- **ChatCompletion** (e.g., gpt-3.5, gpt-4, gpt-4o-mini etc.): Easily integrate conversational chat AI.
- **Embedding**: Obtain vector embeddings for any text input.
- **ImageGeneration** (DALL-E): Generate images from text prompts.
- **Signal/Slot Based**: Each request is asynchronous, emitting signals when data is ready or an error occurs.

---

## Requirements

- **Qt 6** or later (with Core and Network modules).
- **CMake 3.16** or higher.
- A C++17-compatible compiler.
- Valid [OpenAI API key](https://platform.openai.com/account/api-keys).

---

## Project Structure

```
QtOpenAI/
├── CMakeLists.txt
├── README.md
├── src/
│   ├── qtopenai.cpp
│   ├── qtopenai.h
│   ├── qtopenai_global.h
│   ├── network/
│   │   ├── networkmanager.cpp
│   │   └── networkmanager.h
│   └── models/
│       ├── chatcompletion.cpp
│       ├── chatcompletion.h
│       ├── embedding.cpp
│       ├── embedding.h
│       ├── imagegeneration.cpp
│       ├── imagegeneration.h
└── examples/
    └── chat/
        ├── CMakeLists.txt
        └── main.cpp
```

---

## Building

### 1. Using Qt Creator

1. Open **Qt Creator**.
2. Click **File** → **Open File or Project**, select the top-level `CMakeLists.txt`.
3. Choose the appropriate kit (e.g., MSVC, MinGW, etc.).
4. Configure and build.
   - The library (`QtOpenAI.dll`/`.lib` on Windows or `.so` on Linux) is generated in your build output directory.
5. To run the example, set the `examples/chat` project as the startup project and click **Run**.

### 2. Using CMake Command Line

```bash
cd QtOpenAI
mkdir build
cd build
cmake -G "Ninja" -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
```

- On Windows, `QtOpenAI.dll` and `QtOpenAI.lib` will appear in `build/bin` or `build/lib`.
- On Linux, `libQtOpenAI.so` will be generated.
- The example app (`examples/chat`) also gets built.

---

## Usage

1. **Include and link** the **QtOpenAI** library in your project’s `CMakeLists.txt` or `.pro` file.
2. **Create** an instance of `QtOpenAI` and **set your API key**:
   ```cpp
   QtOpenAI openai;
   openai.setApiKey("YOUR_API_KEY");
   ```
3. **ChatCompletion** usage (basic snippet):
   ```cpp
   ChatCompletion* chat = openai.chat();
   QJsonArray messages;
   messages.append(QJsonObject{{"role", "user"}, {"content", "Hello, how are you?"}});
   chat->send(messages);

   QObject::connect(chat, &ChatCompletion::ready, [&](const QString &result) {
       // handle response
   });
   ```
4. **Embedding** usage (basic snippet):
   ```cpp
   Embedding* embed = openai.embeddings();
   embed->getEmbedding("Hello world");

   QObject::connect(embed, &Embedding::embeddingReady, [&](const QJsonArray &emb) {
       // handle embedding result
   });
   ```
5. **ImageGeneration** usage (basic snippet):
   ```cpp
   ImageGeneration* imageGen = openai.image();
   imageGen->generateImage("A cute cat wearing a hat", "1024x1024", 1);

   QObject::connect(imageGen, &ImageGeneration::imageReady, [&](const QList<QUrl> &urls) {
       // handle generated image URLs
   });
   ```

For a complete example, see [`examples/chat/main.cpp`](examples/chat/main.cpp).

---

## OpenRouter Usage

**ChatCompletion** usage (basic snippet):
   ```cpp
   QtOpenAI openai;
   openai.setApiKey("OPENROUTER_API_KEY");

   ChatCompletion* chat = openai.chat();

   chat->setEndPoint("https://openrouter.ai/api/v1/chat/completions");
   chat->setModel("deepseek/deepseek-r1");

   QJsonArray messages;
   messages.append(QJsonObject{{"role", "user"}, {"content", "Hello, how are you?"}});
   chat->send(messages);

   QObject::connect(chat, &ChatCompletion::ready, [&](const QString &result) {
       // handle response
   });
   ```

---

## Potential Issues

- **`.lib`**** not generated on Windows**: Ensure you have the export macro set up (`QTOPENAI_EXPORT`) and the `target_compile_definitions(QtOpenAI PRIVATE QTOPENAI_LIBRARY)` line in `CMakeLists.txt`.
- **Unresolved external symbols**: Check that `qtopenai_global.h` is properly included and that any classes you want to export have the `QTOPENAI_EXPORT` macro.
- **API errors**: Make sure your [OpenAI API key](https://platform.openai.com/account/api-keys) is valid and you have an active internet connection.

---

## License

This project is licensed under the terms of the MIT License. See the [LICENSE](LICENSE) file for details.

---

## Contributions

Contributions are welcome! Please open an issue for bug reports or feature requests. Feel free to submit pull requests with your improvements.

