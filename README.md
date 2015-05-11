NAME
===========

NAME! USACO YEAR

## Have questions?

~~Try the [FAQ/Wiki](http://github.com/usaco/NAME/wiki).~~

## Need updates?

If you need to update your local copy, navigate to `NAME/` and try:

```bash
git pull origin master
```

## What is NAME?

TODO

## How do I get started?

Begin by cloning a copy of the Git repo:

```bash
git clone git://github.com/usaco/NAME.git
```

Go to the `base/` directory and compile the driver:

```bash
cd NAME/base
make
```

This will create the NAME driver.

Next, build the example bots to help with testing:

```bash
cd ../bots
bash build-all.sh
```

Make a new bot to begin working. A helper script has been provided for this purpose.

```bash
cd ..
bash setup-bot.sh "My Awesome Bot"
```

This will create a new directory `MyAwesomeBot/` in the directory `bots/`.
