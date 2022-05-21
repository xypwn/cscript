# cscript

A simple yet powerful script to run C code like a scripting language. Supports multiple files and the inclusion of libraries.

## Table of Contents
1. [Installation](#installation)
2. [Examples](#example-simple)
    1. [Example (simple)](#example-simple)
    2. [Example (with GTK4)](#example-with-gtk4)
    3. [Example (multiple files)](#example-multiple-files)

## Installation
Run the following shell command to install cscript:
```sh
sudo curl -L 'https://git.nobrain.org/r4/cscript/raw/branch/master/cscript' -o /usr/local/bin/cscript && sudo chmod a+rx /usr/local/bin/cscript
```

You can uninstall it by running:
```sh
sudo rm -f /usr/local/bin/cscript
```

## Example (simple)
#### example-simple.c
```c
#!cscript -s
#include <stdio.h>

int main(int argc, char *argv[])  {
	printf("Program name: %s\n", argv[0]);

	if (argc <= 1) {
		printf("No arguments given\n");
	} else {
		for (size_t i = 1; i < argc; i++) {
			printf("Argument %zu: %s\n", i, argv[i]);
		}
	}
}
```

### Indirectly (shebang not required)
#### Run without arguments
```sh
cscript example-simple.c
```

#### Pass arguments to the C program
```sh
cscript example-simple.c -- arg1 arg2 arg3
```

#### Pass arguments to the compiler
```sh
cscript example-simple.c -Ofast
```

### Directly (compiler args passed via shebang)
First we make the file executable: `chmod +x example-simple.c`

#### Run without arguments
```sh
./example-simple.c
```

#### Arguments are now passed to the C program directly
```sh
./example-simple.c arg1 arg2 arg3
```

#### Compiler arguments are passed via the shebang
At the beginning of the C file, write:
```
#!cscript -s -Ofast
```
The `-s` flag puts the "interpreter" in shebang mode.
Any arguments following `-s` in the shebang line are interpreted as further source files or compiler arguments.

## Example (with GTK4)
For this one you'll need the GTK4 development libraries. Install them via `sudo apt install libgtk-4-dev` on anything Debian/Ubuntu.

#### example-gtk4.c
```c
#!cscript -s $(pkg-config --cflags --libs gtk4)
#include <gtk/gtk.h>

static void
print_hello (GtkWidget *widget,
             gpointer   data)
{
  g_print ("Hello World\n");
}

static void
activate (GtkApplication *app,
          gpointer        user_data)
{
  GtkWidget *window;
  GtkWidget *button;
  GtkWidget *box;

  window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "Window");
  gtk_window_set_default_size (GTK_WINDOW (window), 200, 200);

  box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
  gtk_widget_set_halign (box, GTK_ALIGN_CENTER);
  gtk_widget_set_valign (box, GTK_ALIGN_CENTER);

  gtk_window_set_child (GTK_WINDOW (window), box);

  button = gtk_button_new_with_label ("Hello World");

  g_signal_connect (button, "clicked", G_CALLBACK (print_hello), NULL);
  g_signal_connect_swapped (button, "clicked", G_CALLBACK (gtk_window_destroy), window);

  gtk_box_append (GTK_BOX (box), button);

  gtk_widget_show (window);
}

int
main (int    argc,
      char **argv)
{
  GtkApplication *app;
  int status;

  app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}
```

We make the C file executable again: `chmod +x example-gtk4.c`

#### Now just run the script; library inclusion is already handled via the shebang
```sh
./example-gtk4.c
```

#### When not using the shebang (i.e. executing the file indirectly), we need to specify the libraries manually
```sh
cscript example-gtk4.c $(pkg-config --cflags --libs gtk4)
```

## Example (multiple files)
#### example-multi-base.c
```c
#!cscript -s example-multi-ext.c
#include <stdio.h>

extern const char *external_string;

int main()  {
	printf("%s\n", external_string);
}
```

#### example-multi-ext.c
```c
const char *external_string = "Hello, world!";
```

Now we just make example-multi-base.c executable: `chmod +x example-multi-base.c`

#### Again, all the details are handled by the shebang argument
```sh
./example-multi-base.c
```

#### We can also just run multiple C files indirectly
```sh
cscript example-multi-base.c example-multi-ext.c
```
