import tkinter as tk
from tkinter import ttk
from tkinter import filedialog
from tkinter import messagebox
import subprocess
import os
import sys
import threading

class CompressorGUI:
    def __init__(self):
        self.root = tk.Tk()
        self.root.title("Text File Compressor")
        self.root.geometry("400x300")
        
        if getattr(sys, 'frozen', False):
            self.application_path = os.path.dirname(sys.executable)
        else:
            self.application_path = os.path.dirname(os.path.abspath(__file__))
            
        self.setup_compression_window()
        
    def setup_compression_window(self):
        for widget in self.root.winfo_children():
            widget.destroy()

        header = tk.Label(
            self.root,
            text="Text File Compression/Decompression",
            font=("Arial", 12, "bold"),
            bg='#f0f0f0',
            pady=20
        )
        header.pack()
        
        self.root.configure(bg='#f0f0f0')

        op_frame = tk.Frame(self.root, bg='#f0f0f0')
        op_frame.pack(fill='x', padx=20)

        operation = tk.StringVar(value="compress")

        compress_radio = ttk.Radiobutton(
            op_frame,
            text="Compress",
            variable=operation,
            value="compress"
        )
        compress_radio.pack(side='left', padx=20)

        decompress_radio = ttk.Radiobutton(
            op_frame,
            text="Decompress",
            variable=operation,
            value="decompress"
        )
        decompress_radio.pack(side='left', padx=20)

        input_frame = tk.Frame(self.root, bg='#f0f0f0')
        input_frame.pack(fill='x', padx=20, pady=10)

        input_path = tk.StringVar()
        input_entry = ttk.Entry(input_frame, textvariable=input_path, width=30)
        input_entry.pack(side='left', padx=5)

        input_btn = ttk.Button(
            input_frame,
            text="Browse Input",
            command=lambda: input_path.set(filedialog.askopenfilename())
        )
        input_btn.pack(side='left', padx=5)

        output_frame = tk.Frame(self.root, bg='#f0f0f0')
        output_frame.pack(fill='x', padx=20, pady=10)

        output_path = tk.StringVar()
        output_entry = ttk.Entry(output_frame, textvariable=output_path, width=30)
        output_entry.pack(side='left', padx=5)

        output_btn = ttk.Button(
            output_frame,
            text="Browse Output",
            command=lambda: output_path.set(filedialog.asksaveasfilename())
        )
        output_btn.pack(side='left', padx=5)

        process_btn = ttk.Button(
            self.root,
            text="Start Process",
            command=lambda: self.process_text(operation.get(), input_path.get(), output_path.get())
        )
        process_btn.pack(pady=20)

    def show_success_window(self, output_text):
        for widget in self.root.winfo_children():
            widget.destroy()
            
        # Main success message
        success_label = tk.Label(
            self.root,
            text="Operation Completed Successfully!",
            font=("Arial", 12, "bold"),
            bg='#f0f0f0',
            pady=10
        )
        success_label.pack()
        
        # Display the complete output in a text widget
        output_text_widget = tk.Text(
            self.root,
            height=10,
            width=40,
            wrap=tk.WORD
        )
        output_text_widget.pack(pady=10, padx=20)
        output_text_widget.insert(tk.END, output_text)
        output_text_widget.config(state='disabled')
        
        # Home button
        home_btn = ttk.Button(
            self.root,
            text="New Operation",
            command=self.setup_compression_window
        )
        home_btn.pack(pady=10)

    def process_text(self, operation, input_path, output_path):
        if not input_path or not output_path:
            messagebox.showerror("Error", "Please select both input and output files")
            return
            
        try:
            def run_compression():
                try:
                    # Create a temporary input file for the compressor program
                    temp_input_file = "temp_input.txt"
                    with open(temp_input_file, "w") as f:
                        f.write(operation + "\n")
                        f.write(input_path + "\n")
                        f.write(output_path + "\n")
                    
                    # Run the compressor executable
                    compressor_path = os.path.join(self.application_path, "compressor.exe")
                    
                    if not os.path.exists(compressor_path):
                        self.root.after(0, lambda: messagebox.showerror("Error", 
                                        f"Compressor executable not found at: {compressor_path}"))
                        return
                    
                    # Launch the compressor process and redirect input from the temp file
                    process = subprocess.Popen(
                        [compressor_path],
                        stdin=open(temp_input_file, "r"),
                        stdout=subprocess.PIPE,
                        stderr=subprocess.PIPE,
                        text=True
                    )
                    
                    stdout, stderr = process.communicate()
                    
                    # Clean up the temporary file
                    if os.path.exists(temp_input_file):
                        os.remove(temp_input_file)
                    
                    if process.returncode != 0:
                        self.root.after(0, lambda: messagebox.showerror("Error", 
                                        f"Process failed: {stderr}"))
                        return
                    
                    # Show success window
                    self.root.after(0, lambda: self.show_success_window(stdout or "Operation completed successfully"))
                    
                except Exception as e:
                    self.root.after(0, lambda: messagebox.showerror("Error", 
                                    f"An error occurred: {str(e)}"))
            
            # Create a thread to run the compression process
            threading.Thread(target=run_compression).start()
            
        except Exception as e:
            messagebox.showerror("Error", f"An error occurred: {str(e)}")

    def run(self):
        self.root.mainloop()

if __name__ == "__main__":
    app = CompressorGUI()
    app.run()