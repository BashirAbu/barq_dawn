SKIP: FAILED


static uint var_1 = 0u;
void main_1() {
  {
    while(true) {
      var_1 = 1u;
      if (false) {
        break;
      }
      {
        var_1 = 2u;
      }
      continue;
    }
  }
}

void main() {
  main_1();
}

FXC validation failure:
<scrubbed_path>(5,11-14): error X3696: infinite loop detected - loop never exits


tint executable returned error: exit status 1
