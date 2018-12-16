#!/usr/bin/awk -f

BEGIN {
    # primeiro ARGV eh awk
    # for (i = 0; i < ARGC; i++) {
    #     print ARGV[i];
    # }
    texfile = ARGV[1]
    auxfile = "draw/aux";
    
    while ((getline t < texfile) > 0) {
        if (match(t, /^\[/)) {
            break;
        }
        print t
    }
    while ((getline p < auxfile) > 0) {
        match(p, /\{([0-9]+),([0-9]+)\}/, arr);
        # em arr[1] tenho o ID, em arr[2] a folha dele
        pat = "\\[" arr[1] ","
        pat2 = "\\[" arr[1] "\\["
        ret = sub(pat, "[{" arr[1] "," arr[2] "},", t)
        if (ret == 0) {
            sub(pat2, "[{" arr[1] "," arr[2] "}[", t)
        }
    }
    print t
    while ((getline t < texfile) > 0) {
        print t
    }
}
