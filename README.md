# DocGen Lite

DocGen Lite — промежуточная версия генератора документации для C++ проектов. Приложение сканирует `.cpp`, `.h`, `.hpp` файлы, извлекает документирующие комментарии `///` и `/** ... */`, связывает их с ближайшими функциями или классами и генерирует HTML-документацию.

## Структура проекта

```text
.
├── CMakeLists.txt
├── README.md
├── include/docgen/      # заголовочные файлы
└── src/                 # реализации .cpp и main.cpp
```

Логические модули:

- `core` — фасад генератора документации;
- `filesystem` — сканирование и обход файлов;
- `parser` — парсеры C++/header файлов и фабрика парсеров;
- `model` — структуры данных документации;
- `generator` — стратегии генерации HTML и HTML builder;
- `observer` — уведомления о прогрессе;
- `utils` — вспомогательные функции для строк.

## Использованные паттерны

- **Facade** — `DocumentationGenerator` скрывает внутренние этапы генерации.
- **Factory Method** — `ParserFactory` создаёт парсер по расширению файла.
- **Strategy** — `SimpleHtmlStrategy` и `AdvancedHtmlStrategy` выбирают формат генерации.
- **Builder** — `HtmlBuilder` пошагово собирает HTML.
- **Iterator** — `FileIterator` организует обход найденных файлов.
- **Observer** — `GenerationSubject` уведомляет о ходе генерации.

## Сборка

Требуется CMake версии 3.10 или новее и компилятор C++17.

```bash
cmake -S . -B build
cmake --build build
```

## Пример запуска

```bash
cd build
./docgen_lite ../examples ./docs auto simple
```

Аргументы:

```text
./docgen_lite <input_path> <output_path> [file_type] [generation_type]
```

- `input_path` — файл или папка с исходным кодом;
- `output_path` — папка для сохранения HTML-документации;
- `file_type` — `auto`, `.cpp`, `.h`, `.hpp`;
- `generation_type` — `simple` или `advanced`.

Если запустить программу без аргументов, она попросит ввести параметры в консоли.

## Сторонние библиотеки

Сторонние библиотеки не используются. Проект основан только на стандартной библиотеке C++17.

## Docker

Инструкция по сборке образа, запуску тестов внутри контейнера и передаче
аргументов командной строки находится в файле [DOCKER.md](DOCKER.md).
