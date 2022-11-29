package casmParser;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.stream.Stream;

import javax.swing.text.html.StyleSheet;

public class Casmparser {

    public static final String[] INSTRUCTIONS = {
        "do_nothing",
        "mov_reg_u64",
        "mov_reg_reg",
        "mov_reg_mem",
        "mov_mem_u64",
        "mov_mem_reg",
        "push",
        "pop",
        "syscall",
        "jmp",
        "jmpz",
        "jmpnz",
        "cmp",
        "call",
        "ret",
        "add_reg_u64",
        "add_reg_reg",
        "add_reg_mem",
        "add_mem_u64",
        "add_mem_reg"
    };
    
    public static final String[] REGISTERS = {
        "$RAX",
        "$RBX",
        "$RCX",
        "$RDX",
        "$RSX"
    };

    public static final String[] CMPS = {
            "==",
            "<",
            ">",
            "<=",
            ">="
    };

    public static byte[] reverse(byte[] array) {
        if (array == null) {
            return array;
        }
        int i = 0;
        int j = array.length - 1;
        byte tmp;
        while (j > i) {
            tmp = array[j];
            array[j] = array[i];
            array[i] = tmp;
            j--;
            i++;
        }
        return array;
    }
  

    static byte[] longToByteArray(long value) {
        return reverse(ByteBuffer.allocate(8).putLong(value).array());
    }

    public static String expandNum(long num) {
        byte[] bytes = longToByteArray(num);
        String s = "";
        for(int i =0; i < 7; i++) {    
            s += Byte.toUnsignedInt(bytes[i]);
            s += ',';
        }
        s+=Byte.toUnsignedInt(bytes[7]);
        return s;
    }

    public static String parseWord(String word) {
        if(word.matches("-?\\d+")) return expandNum(Long.parseLong(word));
        for (int i = 0; i < INSTRUCTIONS.length; i++) {
            if(INSTRUCTIONS[i].equals(word)) return Integer.toString(i);
            if(i >= REGISTERS.length) continue;
            if(REGISTERS[i].equals(word)) return Integer.toString(i);
            if(i >= CMPS.length) continue;
            if(CMPS[i].equals(word)) return Integer.toString(i);
        }
        return word;
    }

    public static String parseLine(String line) {
        String s = "";
        String[] sS = line.trim().split(" ");
        for (String string : sS) {
            s += parseWord(string) + ",";
        }
        return s;
    }

    public static List<String> getLabels(String byteCode) {
        List<String> labels = new ArrayList<String>();
        for (String s : byteCode.split(",")) {
            if(s.endsWith(":")) labels.add(s.substring(0, s.length()-1));
        }
        return labels;
    }

    public static void main(String[] args) {
        String path = (args.length >0)? args[0] : "../example.casm";
        String text = "";
        try {
            text = new String(Files.readAllBytes(Paths.get(path)), StandardCharsets.UTF_8);
        } catch (IOException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
        List<String> lines = Arrays.asList(text.split("\n"));
        lines = lines.stream().filter(line -> !line.isEmpty()).toList();
        String byteCode = "";
        for (String line : lines) {
            byteCode += parseLine(line);
        }
        //System.out.println(byteCode);
        List<String> labels = getLabels(byteCode);
        HashMap<Integer,String> locOfLabel = new HashMap<Integer,String>();
        String[] buff = byteCode.split(",");
        byteCode = "";
        for (int i = 0; i < buff.length; i++) {
            for (int j = 0; j < labels.size(); j++) {
                if(buff[i].equals(labels.get(j))) {
                    locOfLabel.put(i, labels.get(j));
                    buff[i] = expandNum(0);
                }
            }
            byteCode += buff[i] + ",";
        }
        HashMap<String,Integer> posOfLabel = new HashMap<String,Integer>();
        buff = byteCode.split(",");
        byteCode = "";
        for (int i = 0; i < buff.length; i++) {
            if(buff[i].endsWith(":")) {
                posOfLabel.put(buff[i].substring(0,buff[i].length()-1), i);
                buff[i] = "0";
            }
            byteCode += buff[i] + ",";
        }
        byteCode = finaliseByteCode(byteCode,locOfLabel,posOfLabel);
        byteCode += "255";
        if(!byteCode.matches("^[0-9,]*$")) { System.exit(-1); }
        System.out.println(byteCode);
    }

    //FIXME: Loops are broken sometimes depending on where they start

    private static String finaliseByteCode(String byteCode, HashMap<Integer,String> locOfLabel, HashMap<String,Integer> posOfLabel) {
        String[] splt = byteCode.split(",");
        int count = 0;
        for (Integer pos : locOfLabel.keySet()) {
            String label = locOfLabel.get(pos);
            String s = expandNum(Long.valueOf(posOfLabel.get(label)));
            //System.out.println("["+label+"] : { loc:" + posOfLabel.get(label) + "; index: " + (pos+(7*count)) + "; put: " + s + "; }");
            String[] replaceString = s.split(","); 
            for(int i = 0; i < 8; i++) {
                splt[pos+(7*count)+i] = replaceString[i];
            }
            count++;
        }
        byteCode="";
        for (int i = 0; i < splt.length; i++) {
            byteCode += splt[i] + ",";
        }
        return byteCode;
    }
}