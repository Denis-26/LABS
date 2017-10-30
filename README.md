# LABS

### Для работы с файловой системой

1) **./app** -w *filename.txt* - записывает существующий filename.txt в систему.
2) **./app** -r *filename.txt* - выводит содержимое файла filename.txt , если он есть в системе
3) **./app** -c *filename1.txt filename2.txt* - копирует содержимое файла filename1.txt, создает в файловой системе новый файл с именем filename2.txt и записывает содержимое файла filename1.txt в filename2.txt, если filename1.txt существует, а filename2.txt нет
4) **./app** -r *filename.txt* - удаляет файл filename.txt из файловой системы, если тот существует.
5) **./app** -l - выводит список файлов в файловой системе.
