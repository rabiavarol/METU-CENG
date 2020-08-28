package ceng.ceng351.labdb;

import java.util.*;

public class LabDB {
    public int global_depth = 0;
    ArrayList<BucketNode> table;
    private class BucketNode{
        private int local_depth;
        private int size;
        LinkedList<String> students = new LinkedList<String>();
        public BucketNode(int bucket_size, int ld) {
            size = bucket_size;
            local_depth = ld;
        } 
    }
    public LabDB(int bucketSize) {
        this.global_depth = 1;
        this.table = new ArrayList<BucketNode>();
        table.add(new BucketNode(bucketSize,1));
        table.add(new BucketNode(bucketSize,1));
    }
    public int hash_func(String studentID){
        String str_sid = studentID.substring(1); 
        int sid = Integer.parseInt(str_sid); 
        String bin_sid = Integer.toBinaryString(sid); 
        while(bin_sid.length() < global_depth) bin_sid = "0" + bin_sid;
        String last_k_bits = bin_sid.substring(bin_sid.length()-global_depth,bin_sid.length());
        int index = Integer.parseInt(last_k_bits,2);
        return index;
    }
    public int pair_index(int bucket_no, int local_depth){
        return bucket_no^(1 << (local_depth-1));
    }
    public void shrink_table(ArrayList<BucketNode> table){
        int new_table_size;
        int table_size = table.size();
        for(int i = 0; i < table_size; i++){
            if(table.get(i).local_depth==global_depth) //if any bucket has a local depth equals to the global depth
                return;
        }
        global_depth--;
        new_table_size = 1 << global_depth;
        for(int i = 0; i < new_table_size;i++){
            table.remove(table.size()-1);
        }
    }
    private void enlarge_table(ArrayList<BucketNode> table){
        int table_size = table.size();
        for(int i = table_size; i < 2*table_size; i++){
            table.add(table.get(i-table_size));
        }
        global_depth++;
    }
    public void split_bucket(int bucket_no,BucketNode data_bucket){
        String temp;
        int temp_students_size;
        int pair_ind, index_diff, i;
        LinkedList<String> temp_students = new LinkedList<String>();
        pair_ind = pair_index(bucket_no, data_bucket.local_depth);
        index_diff = 1 << data_bucket.local_depth;
        BucketNode new_bucket = new BucketNode(data_bucket.size,data_bucket.local_depth);
        table.set(pair_ind, new_bucket);
        for(i = 0; i < data_bucket.students.size(); i++){
            temp_students.add(i, data_bucket.students.get(i));
        }
        temp_students_size = temp_students.size();
        data_bucket.students.clear();
        for(i = pair_ind-index_diff; i >= 0; i-=index_diff){
            table.set(i, table.get(pair_ind));
        }
        for(i = pair_ind+index_diff; i < table.size(); i+=index_diff){
            table.set(i, table.get(pair_ind));
        }
        for(i = 0; i < temp_students_size; i++){
            temp = temp_students.removeFirst();
            enter(temp);
        } 
    }
    public void enter(String studentID) {
        String exist = search(studentID);
        if(exist == "-1"){ //TEST THIS
            int bucket_no = hash_func(studentID);
            BucketNode data_bucket = table.get(bucket_no);
            data_bucket.students.add(studentID);
            if(data_bucket.students.size() > data_bucket.size){
            //bucket overflow
                if(data_bucket.local_depth == global_depth){
                    //table expansion
                    enlarge_table(table);
                }
                data_bucket.local_depth++;
                split_bucket(bucket_no, data_bucket);
            }
        }
    }
    public boolean merge_bucket(int bucket_no){
        BucketNode data_bucket, buddy_bucket;
        int i, pair_ind, index_diff, local_depth, table_size;
        data_bucket = table.get(bucket_no);
        local_depth = data_bucket.local_depth;
        pair_ind = pair_index(bucket_no, local_depth);
        buddy_bucket = table.get(pair_ind);
        table_size = 1 << global_depth;
        index_diff = 1 << local_depth;
        if(local_depth != buddy_bucket.local_depth )// no merge exists
            return false;
        if(buddy_bucket.local_depth == local_depth){ // merge exists
            buddy_bucket.local_depth--;
            table.set(bucket_no, buddy_bucket);
            for(i = (bucket_no-index_diff); i >= 0; i -= index_diff){
                table.set(i, buddy_bucket);
            }
            for(i = (bucket_no + index_diff); i < table_size; i += index_diff){
                table.set(i, buddy_bucket);
            }
        }
        return true;
        

    }

    public void leave(String studentID){
        int bucket_no = hash_func(studentID);
        BucketNode data_bucket = table.get(bucket_no);
        String exist = search(studentID);
        if(exist != "-1"){
            boolean is_removed = data_bucket.students.remove(studentID);
            while(data_bucket.students.size() == 0 && data_bucket.local_depth != 1 && global_depth !=1 && is_removed){
                boolean is_merged = merge_bucket(bucket_no);
                if(!is_merged) break; 
                shrink_table(table);
                bucket_no = hash_func(studentID); // since new bucket no can be changed 
                data_bucket = table.get(bucket_no);
            }
        }
        
    }

    public String search(String studentID) {
        boolean status;
        String table_ind;
        int index;
        index = hash_func(studentID);
        BucketNode data_bucket = table.get(index);
        status = data_bucket.students.contains(studentID);
        table_ind = Integer.toBinaryString(index);
        while(table_ind.length() < global_depth){
            table_ind = "0" + table_ind;
        }
        if(status)
            return table_ind;
        return "-1";

    }
    public String ll_formatter(LinkedList<String> students){
        int size = students.size();
        String result = "";
        for(int i = 0; i < size; i++){
            result += "<" + students.get(i) + ">";
        }
        return result;
    }
    public void printLab() {
        int table_size,i;
        String table_ind;
        table_size = 1 << global_depth;
        System.out.println("Global depth : " + global_depth);
        for(i = 0; i < table_size; i++){
            table_ind = Integer.toBinaryString(i);
            while(table_ind.length() < global_depth){
                table_ind = "0" + table_ind;
            }
            System.out.println(table_ind + " : [Local depth:" + table.get(i).local_depth + "]" + ll_formatter(table.get(i).students));
        }
        
        
    }
}
