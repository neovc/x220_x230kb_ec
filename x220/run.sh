rm -f 8duj31us.iso.*
if [ -f x220/8duj31us.iso.orig ]; then
	cp -f x220/8duj31us.iso.orig 8duj31us.iso.orig
else
	wget -O 8duj31us.iso.orig https://download.lenovo.com/pccbbs/mobiles/8duj31us.iso
fi

cp -f 8duj31us.iso.orig 8duj31us.iso.tmp

./scripts/ISO_copyFL2 from_iso 8duj31us.iso.orig x220.FL2.orig 01CB000.FL2
./scripts/FL2_copyIMG from_fl2 x220.FL2.orig x220.ec.img.tmp
cp -f x220.FL2.orig x220.FL2.tmp
cp -f x220.ec.img.tmp x220.ec.img.orig

./scripts/hexpatch.pl --rm_on_fail --fail_on_missing --report x220.report x220.ec.img.tmp x220/001_keysym.patch x220/002_dead_keys.patch x220/003_keysym_replacements.patch

./scripts/FL2_copyIMG to_fl2 x220.FL2.tmp x220.img.tmp

./scripts/ISO_copyFL2 to_iso 8duj31us.iso.tmp x220.FL2.tmp 01CB000.FL2

./scripts/ISO_copyFL2 to_iso 8duj31us.iso.tmp x220/s01CB000.whitelist.FL1 01CB000.FL1

./scripts/geteltorito -o patched.x220.img 8duj31us.iso.tmp

#./scripts/fix_mbr patched.x220.img
