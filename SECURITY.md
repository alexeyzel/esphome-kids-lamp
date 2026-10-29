# Privacy and security / Приватність і безпека

## English

Review date: 2026-10-29. Tracked source files were inspected for personal names, email addresses, local paths, network identifiers and credential patterns. Child names were replaced by neutral zone names in firmware identifiers and documentation. No real Wi-Fi passwords, API keys or tokens were found in the current tracked files; `secrets.yaml.example` contains placeholders. This is a manual review, not a guarantee of absence of every possible identifier.

**Remaining exposure:** existing Git commit metadata contains an author's real name and email; earlier revisions contain the former personal lamp names. The public GitHub account identifier remains in functional package URLs. Working-tree edits do not erase any of these historical copies, remote clones or caches. History was not rewritten or force-pushed during this review; the repository must not be described as fully anonymous.

Keep credentials in local `secrets.yaml`; `.gitignore` also excludes environments, build output and logs. Ignored local files are outside the reviewed publishable source set. Check CAD exports for embedded author/path metadata before uploading. Renaming zones may change HA-generated entity identifiers; review dashboards and automations after updating. Gesture and lighting behavior is unchanged.

If a real credential is ever published, revoke/rotate it first; deleting it in a later commit is insufficient. Report security findings privately through an available maintainer contact rather than posting credentials in public issues.

## Українська

Дата перевірки: 2026-10-29. Перевірено відстежувані вихідні файли на імена, email, локальні шляхи, мережеві ідентифікатори та шаблони секретів. Імена дітей замінено нейтральними зонами у прошивці й документації. Реальних Wi-Fi-паролів, API-ключів чи токенів у поточних відстежуваних файлах не виявлено; `secrets.yaml.example` містить шаблони. Ручна перевірка не гарантує відсутності всіх можливих ідентифікаторів.

**Що залишається:** метадані попередніх комітів Git містять справжні ім'я та email автора, а старі версії — персональні назви ламп. Публічний ідентифікатор GitHub залишено в робочих URL пакетів. Зміни поточних файлів не видаляють історію, віддалені клони чи кеші. Історію не переписано, force-push не виконано; репозиторій не можна вважати повністю анонімним.

Секрети зберігайте в локальному `secrets.yaml`. `.gitignore` також виключає середовища, збірки й логи. Ігноровані локальні файли не входять у перевірений набір джерел для публікації. Перед додаванням CAD перевіряйте метадані автора та шляхів. Перейменування зон може змінити згенеровані HA ідентифікатори: перевірте панелі й автоматизації після оновлення. Поведінку кнопки та світла не змінено.

Якщо реальний секрет опубліковано, спершу відкличте/замініть його: видалення наступним комітом недостатньо. Повідомляйте про вразливості приватним доступним каналом зв'язку з власником, не публікуючи секрети в issues.
