# 🚀 Инструкция по сохранению проекта в GitLab

## 📋 Быстрая инструкция

### Вариант 1: Создание нового репозитория на GitLab

#### Шаг 1: Создайте репозиторий на GitLab

1. Откройте https://gitlab.com (или ваш GitLab сервер)
2. Войдите в аккаунт
3. Нажмите **New project** → **Create blank project**
4. Заполните:
   - **Project name**: `esp32-ph-sensor-mqtt`
   - **Visibility**: Public или Private
   - **Initialize with README**: ❌ НЕ ставьте галочку!
5. Нажмите **Create project**

#### Шаг 2: Инициализируйте Git в проекте

```powershell
# Перейдите в папку проекта
cd e:\cursor\PH

# Инициализируйте Git
git init

# Добавьте все файлы
git add .

# Создайте первый коммит
git commit -m "Initial commit: ESP32 pH Sensor Monitor with MQTT"

# Добавьте удаленный репозиторий (замените YOUR_USERNAME и YOUR_PROJECT)
git remote add origin https://gitlab.com/YOUR_USERNAME/esp32-ph-sensor-mqtt.git

# Отправьте код в GitLab
git push -u origin master
```

---

## 📝 Подробная инструкция

### Шаг 1: Создайте .gitignore файл

```powershell
cd e:\cursor\PH
```

Создайте файл `.gitignore` со следующим содержимым:

```gitignore
# PlatformIO
.pio
.vscode/.browse.c_cpp.db*
.vscode/c_cpp_properties.json
.vscode/launch.json
.vscode/ipch
.vscode
.pioenvs
.piolibdeps

# Compiled files
*.o
*.obj
*.elf
*.bin
*.hex

# Temporary files
*~
*.swp
*.tmp
*.bak
temp/

# OS files
.DS_Store
Thumbs.db

# Secrets
secrets.yaml
.env
```

### Шаг 2: Инициализируйте Git репозиторий

```powershell
# Инициализация
git init

# Настройте имя и email (если еще не настроено)
git config user.name "Your Name"
git config user.email "your.email@example.com"

# Проверьте статус
git status
```

### Шаг 3: Добавьте файлы в Git

```powershell
# Добавьте все файлы
git add .

# Проверьте, что будет закоммичено
git status

# Создайте первый коммит
git commit -m "Initial commit: ESP32 pH Sensor Monitor

- Основная программа с MQTT и Wi-Fi
- Полная документация на русском
- Схемы подключения
- Руководство по калибровке
- Интеграция с Home Assistant
- Тестовая программа"
```

### Шаг 4: Создайте репозиторий на GitLab

#### Через веб-интерфейс:

1. Откройте https://gitlab.com
2. Войдите в аккаунт
3. Нажмите **+** → **New project/repository**
4. Выберите **Create blank project**
5. Заполните форму:
   ```
   Project name: esp32-ph-sensor-mqtt
   Project slug: esp32-ph-sensor-mqtt
   Visibility Level: Public (или Private)
   Project Configuration:
     ☐ Initialize repository with a README
     ☐ Enable Static Application Security Testing (SAST)
   ```
6. Нажмите **Create project**

### Шаг 5: Подключите локальный репозиторий к GitLab

После создания проекта GitLab покажет инструкции. Выполните:

```powershell
# Добавьте удаленный репозиторий
# Замените YOUR_USERNAME на ваше имя пользователя
git remote add origin https://gitlab.com/YOUR_USERNAME/esp32-ph-sensor-mqtt.git

# Проверьте удаленный репозиторий
git remote -v

# Отправьте код в GitLab
git push -u origin master
```

### Шаг 6: Проверьте результат

Откройте https://gitlab.com/YOUR_USERNAME/esp32-ph-sensor-mqtt

Вы должны увидеть все файлы проекта!

---

## 🔐 Аутентификация

### Вариант A: HTTPS с Personal Access Token (рекомендуется)

#### Создание токена:

1. GitLab → **Settings** → **Access Tokens**
2. Заполните:
   - **Token name**: `esp32-ph-sensor-token`
   - **Expiration date**: выберите дату
   - **Scopes**: ✅ `write_repository`, ✅ `read_repository`
3. Нажмите **Create personal access token**
4. **Скопируйте токен** (он больше не будет показан!)

#### Использование токена:

```powershell
# При первом push GitLab запросит логин и пароль
# Username: ваш username
# Password: вставьте токен (не пароль!)

git push -u origin master
```

#### Сохранение токена (чтобы не вводить каждый раз):

```powershell
# Windows Credential Manager (рекомендуется)
git config --global credential.helper wincred

# Или сохранить в URL (менее безопасно)
git remote set-url origin https://YOUR_USERNAME:YOUR_TOKEN@gitlab.com/YOUR_USERNAME/esp32-ph-sensor-mqtt.git
```

### Вариант B: SSH ключ

#### Генерация SSH ключа:

```powershell
# Откройте PowerShell
ssh-keygen -t ed25519 -C "your.email@example.com"

# Нажмите Enter для сохранения в стандартное место
# Введите пароль (или оставьте пустым)

# Скопируйте публичный ключ
Get-Content ~\.ssh\id_ed25519.pub | clip
```

#### Добавление ключа в GitLab:

1. GitLab → **Settings** → **SSH Keys**
2. Вставьте скопированный ключ
3. **Title**: `ESP32 Development PC`
4. Нажмите **Add key**

#### Использование SSH:

```powershell
# Измените URL на SSH
git remote set-url origin git@gitlab.com:YOUR_USERNAME/esp32-ph-sensor-mqtt.git

# Теперь можно пушить без пароля
git push -u origin master
```

---

## 📦 Структура репозитория

После загрузки в GitLab структура будет:

```
esp32-ph-sensor-mqtt/
├── README_PH.md                        ← Главный README
├── QUICK_START_PH.md                   ← Быстрый старт
├── README_PH_SENSOR.md                 ← Полная документация
├── PROJECT_SUMMARY.md                  ← Сводка проекта
├── INSTALLATION_CHECKLIST.md           ← Чеклист установки
├── GIT_SETUP.md                        ← Эта инструкция
│
├── ph_calibration_guide.txt            ← Калибровка
├── ph_sensor_wiring.txt                ← Схемы подключения
├── home_assistant_ph_config.yaml       ← Home Assistant
│
├── platformio.ini                      ← Конфигурация PlatformIO
├── .gitignore                          ← Игнорируемые файлы
│
├── src/
│   └── main.cpp                        ← Основная программа
│
├── test/
│   └── simple_test.cpp                 ← Тестовая программа
│
├── include/                            ← Заголовочные файлы
└── lib/                                ← Библиотеки
```

---

## 🔄 Ежедневная работа с Git

### Сохранение изменений

```powershell
# Проверьте изменения
git status

# Добавьте измененные файлы
git add .

# Или добавьте конкретный файл
git add src/main.cpp

# Создайте коммит
git commit -m "Описание изменений"

# Отправьте в GitLab
git push
```

### Примеры коммитов

```powershell
# Исправление бага
git commit -m "Fix: Исправлена ошибка переподключения к MQTT"

# Новая функция
git commit -m "Feature: Добавлена температурная компенсация"

# Обновление документации
git commit -m "Docs: Обновлено руководство по калибровке"

# Изменение конфигурации
git commit -m "Config: Изменен интервал публикации на 60 секунд"
```

### Просмотр истории

```powershell
# История коммитов
git log

# Краткая история
git log --oneline

# История с графом
git log --graph --oneline --all
```

### Отмена изменений

```powershell
# Отменить изменения в файле (до add)
git checkout -- src/main.cpp

# Отменить add (но сохранить изменения)
git reset HEAD src/main.cpp

# Отменить последний коммит (но сохранить изменения)
git reset --soft HEAD~1

# Полностью отменить последний коммит
git reset --hard HEAD~1
```

---

## 🌿 Работа с ветками

### Создание веток для разработки

```powershell
# Создать новую ветку
git checkout -b feature/temperature-compensation

# Переключиться на ветку
git checkout feature/temperature-compensation

# Список веток
git branch

# Удалить ветку
git branch -d feature/temperature-compensation
```

### Слияние веток

```powershell
# Переключитесь на master
git checkout master

# Слейте ветку
git merge feature/temperature-compensation

# Отправьте изменения
git push
```

---

## 📋 Рекомендуемая структура коммитов

### Типы коммитов:

- `feat:` - новая функция
- `fix:` - исправление бага
- `docs:` - изменения в документации
- `style:` - форматирование кода
- `refactor:` - рефакторинг
- `test:` - добавление тестов
- `chore:` - обновление зависимостей, конфигурации

### Примеры:

```powershell
git commit -m "feat: добавлена поддержка OTA обновлений"
git commit -m "fix: исправлена утечка памяти в MQTT клиенте"
git commit -m "docs: обновлена схема подключения"
git commit -m "refactor: улучшена функция калибровки"
git commit -m "chore: обновлена версия ArduinoJson до 6.22"
```

---

## 🏷️ Теги и релизы

### Создание тега

```powershell
# Создать тег для версии
git tag -a v1.0.0 -m "Release version 1.0.0"

# Отправить тег в GitLab
git push origin v1.0.0

# Отправить все теги
git push --tags
```

### Создание релиза на GitLab

1. Откройте проект на GitLab
2. **Deployments** → **Releases**
3. Нажмите **New release**
4. Заполните:
   - **Tag name**: `v1.0.0`
   - **Release title**: `ESP32 pH Sensor v1.0.0`
   - **Release notes**: Описание изменений
5. Нажмите **Create release**

---

## 📄 README для GitLab

GitLab автоматически отобразит `README_PH.md` на главной странице.

Для лучшего отображения переименуйте:

```powershell
# Переименуйте README_PH.md в README.md
git mv README_PH.md README.md
git commit -m "docs: переименован README для GitLab"
git push
```

Или создайте символическую ссылку:

```powershell
# Создайте README.md, который ссылается на README_PH.md
# В README.md напишите:
# См. [README_PH.md](README_PH.md) для полной документации
```

---

## 🔍 GitLab CI/CD (опционально)

Создайте `.gitlab-ci.yml` для автоматической сборки:

```yaml
image: python:3.11

stages:
  - build
  - test

build:
  stage: build
  before_script:
    - pip install platformio
  script:
    - pio run
  artifacts:
    paths:
      - .pio/build/esp32dev/firmware.bin
    expire_in: 1 week

test:
  stage: test
  before_script:
    - pip install platformio
  script:
    - pio test
```

---

## 🛡️ .gitignore для ESP32 проектов

Убедитесь, что `.gitignore` содержит:

```gitignore
# PlatformIO
.pio/
.vscode/
.pioenvs/
.piolibdeps/

# Compiled
*.o
*.elf
*.bin
*.hex

# Secrets
secrets.yaml
.env
wifi_config.h

# OS
.DS_Store
Thumbs.db

# Temporary
*.tmp
*.bak
*~
```

---

## 🚀 Быстрые команды

### Первая загрузка в GitLab

```powershell
cd e:\cursor\PH
git init
git add .
git commit -m "Initial commit: ESP32 pH Sensor Monitor"
git remote add origin https://gitlab.com/YOUR_USERNAME/esp32-ph-sensor-mqtt.git
git push -u origin master
```

### Ежедневное сохранение

```powershell
git add .
git commit -m "Описание изменений"
git push
```

### Клонирование на другой компьютер

```powershell
git clone https://gitlab.com/YOUR_USERNAME/esp32-ph-sensor-mqtt.git
cd esp32-ph-sensor-mqtt
pio run --target upload
```

---

## 🆘 Решение проблем

### Проблема: "fatal: not a git repository"

```powershell
# Решение: инициализируйте Git
git init
```

### Проблема: "remote origin already exists"

```powershell
# Решение: удалите и добавьте заново
git remote remove origin
git remote add origin https://gitlab.com/YOUR_USERNAME/esp32-ph-sensor-mqtt.git
```

### Проблема: "failed to push some refs"

```powershell
# Решение: сначала получите изменения
git pull origin master --allow-unrelated-histories
git push -u origin master
```

### Проблема: Запрос логина/пароля каждый раз

```powershell
# Решение: используйте credential helper
git config --global credential.helper wincred
```

### Проблема: "Permission denied (publickey)"

```powershell
# Решение: проверьте SSH ключ
ssh -T git@gitlab.com

# Если не работает, используйте HTTPS вместо SSH
git remote set-url origin https://gitlab.com/YOUR_USERNAME/esp32-ph-sensor-mqtt.git
```

---

## 📚 Дополнительные ресурсы

### Документация

- **Git**: https://git-scm.com/doc
- **GitLab**: https://docs.gitlab.com/
- **Git на русском**: https://git-scm.com/book/ru/v2

### Обучение

- **Git Basics**: https://git-scm.com/book/en/v2/Getting-Started-Git-Basics
- **GitLab Flow**: https://docs.gitlab.com/ee/topics/gitlab_flow.html

### Инструменты

- **GitKraken**: Графический клиент Git
- **SourceTree**: Бесплатный Git GUI
- **VS Code Git**: Встроенная поддержка Git

---

## ✅ Чеклист

### Перед первой загрузкой:

- [ ] Создан `.gitignore`
- [ ] Удалены секретные данные (пароли, токены)
- [ ] Проверена структура проекта
- [ ] Создан репозиторий на GitLab
- [ ] Настроена аутентификация (токен или SSH)

### После загрузки:

- [ ] Проверен репозиторий на GitLab
- [ ] README отображается корректно
- [ ] Все файлы на месте
- [ ] Создан первый релиз (опционально)
- [ ] Настроен CI/CD (опционально)

---

## 🎉 Готово!

Теперь ваш проект сохранен в GitLab и доступен из любого места!

### Следующие шаги:

1. ⭐ Сделайте репозиторий публичным (если хотите поделиться)
2. 📝 Добавьте описание проекта на GitLab
3. 🏷️ Создайте первый релиз v1.0.0
4. 📢 Поделитесь ссылкой с коллегами

---

**Версия:** 1.0  
**Дата:** Декабрь 2025

