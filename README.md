# Negative-Iqs-Template

### convert crlf to lf
```bash
find -type f -print0 | xargs -0 dos2unix
```

### remove empty lines
```bash
sed -i '/^$/d' ./*
```

