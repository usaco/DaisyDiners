Daisy Diners
===========

Daisy Diners! USACO 2015

## Have questions?

~~Try the [FAQ/Wiki](http://github.com/usaco/DaisyDiners/wiki).~~

## Need updates?

If you need to update your local copy, navigate to `DaisyDiners/` and try:

```bash
git pull origin master
```

## What is Daisy Diners?

Farmer John would like to fence in a rectangular area of his farm to hold his prized daisy collection.  He wants to be careful not to fence in any of his cows during the process, however, since his cows like nothing more than to eat the daisies.
 
Each game happens on an N x N grid representing the farm (N doesn’t matter too much; it could be small or large), and involves K teams playing at the same time (K could be up to 12, let’s say, but I think the ideal setting for K will be something in the 4..6 range).  The game consists of a number of rounds. 
 
In each round, each team’s program independently selects a rectangular area of the farm to propose as the fenced-in area.  Each team also selects a grid cell in which to place one cow.  Points are then awarded: each team that chose a rectangle containing no cows receives points proportional to its area.  Each team whose cow ends up inside other teams’ rectangles receives points equal to the sum of those areas.  If multiple cows are responsible for eliminating a rectangle, we split the points for that rectangle evenly among them.

## How do I get started?

Begin by cloning a copy of the Git repo:

```bash
git clone git://github.com/usaco/DaisyDiners.git
```

Go to the `base/` directory and compile the driver:

```bash
cd DaisyDiners/base
make
```

This will create the DaisyDiners driver.

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
