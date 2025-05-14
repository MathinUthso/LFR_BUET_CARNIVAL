void TeamOne() {
  oled.clear();
  oled.set2X();
  oled.setInvertMode(0);
  text(" PGLU PGLU", 4, 1);
  text(" OhMyPaGlu", 0, 6);
}

void text(String t, byte x, byte y) {
  oled.setCursor(x, y);
  oled.print(t);
}
