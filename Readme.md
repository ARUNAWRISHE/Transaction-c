<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8" />
  <meta name="viewport" content="width=device-width, initial-scale=1.0"/>
  <title>Bank Transactions (C Project)</title>
  <style>
    body {
      font-family: "Segoe UI", sans-serif;
      background-color: #f4f4f4;
      color: #333;
      line-height: 1.6;
      max-width: 900px;
      margin: 30px auto;
      padding: 20px;
      background: #fff;
      border-radius: 10px;
      box-shadow: 0 4px 12px rgba(0,0,0,0.1);
    }
    h1, h2 {
      color: #0074cc;
    }
    code {
      background: #eee;
      padding: 2px 6px;
      border-radius: 5px;
      font-family: monospace;
    }
    pre {
      background: #272822;
      color: #f8f8f2;
      padding: 15px;
      border-radius: 5px;
      overflow-x: auto;
    }
    ul {
      margin-top: 0;
    }
  </style>
</head>
<body>

  <h1>💼 Bank Transactions (C Project)</h1>

  <p>This is a simple <strong>terminal-based program in C</strong> to manage basic bank account operations using binary files. It's a menu-driven application that allows you to <em>add, update, delete, and view account details</em>. All data is stored in a file called <code>credit.dat</code>, and there's an option to export account info to a readable text file.</p>

  <h2>🛠 What It Does</h2>
  <ul>
    <li>Add a new account with name and balance</li>
    <li>Update the balance of an existing account</li>
    <li>Delete an account</li>
    <li>View all accounts in a formatted <code>.txt</code> file</li>
  </ul>

  <h2>📁 Files in This Project</h2>
  <ul>
    <li><code>trans.c</code> – Main C file with all logic</li>
    <li><code>credit.dat</code> – Binary file where account data is stored</li>
    <li><code>accounts.txt</code> – Exported text file of all active accounts</li>
    <li><code>trans.exe</code> – Compiled version for Windows</li>
    <li><code>clients.dat</code> – Not currently used (possibly test or leftover file)</li>
  </ul>

  <h2>💻 How to Compile and Run</h2>

  <h3>Using GCC</h3>
  <pre><code>gcc trans.c -o trans.exe
./trans.exe</code></pre>

  <h3>Using Clang</h3>
  <pre><code>clang trans.c -o transaction
./transaction</code></pre>

  <p>Once compiled, the program launches a simple menu in the terminal where you can interact with the system.</p>

  <h2>📌 Few Details</h2>
  <ul>
    <li>Account numbers range from <strong>1 to 100</strong></li>
    <li>Each account has:
      <ul>
        <li>Account number</li>
        <li>First name</li>
        <li>Last name</li>
        <li>Balance</li>
      </ul>
    </li>
    <li>Adding an account that already exists is blocked (no overwrite)</li>
    <li>Deleting an account simply clears it from the file</li>
    <li>Tested and works on <strong>Windows and Linux</strong> (GCC & Clang)</li>
  </ul>

  <p><strong>✅ Tip:</strong> Make sure your terminal is in the correct directory where <code>trans.c</code> is located before compiling or running.</p>

</body>
</html>
