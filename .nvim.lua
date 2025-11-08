print("Welcome, to Pearl Engine! 🦖");
vim.keymap.set("n", "<leader>bb", ":!cmake --build build<CR>", { desc = "[Pearl] Build project" })
vim.keymap.set("n", "<leader>rr", ":!./build/game_engine<CR>", { desc = "[Pearl] Run executable" })
vim.keymap.set("n", "<leader>br", ":!./build_run.sh<CR>", { desc = "[Pearl] Build and run" })
vim.keymap.set("n", "<leader>cr", ":!./compile_build_run.sh<CR>", { desc = "[Pearl] Compile, build and run" })
vim.keymap.set("n", "<leader>bc", ":!./clean_build.sh<CR>", { desc = "[Pearl] Clean build" })
